# C:\projectsky\ai_server.py - Fixed Version with BoTSORT & Lock Class Tracking

import os
# ตั้ง SMARTPALM_FORCE_CPU=1 ถ้าต้องการบังคับใช้ CPU (เช่น เครื่องที่ไม่มีการ์ดจอ NVIDIA หรือไดรเวอร์มีปัญหา)
FORCE_CPU = os.environ.get("SMARTPALM_FORCE_CPU") == "1"

import sys

# โปรแกรม C++ เปิดไฟล์นี้แบบซ่อนหน้าต่าง จึงมองไม่เห็นข้อความ/error -> เขียนทุกข้อความลงไฟล์ log ด้วย
# (ต้องทำก่อน import ultralytics เพราะ logger ของมันจับ sys.stdout ไว้ตอน import)
LOG_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ai_server_log.txt")

class _Tee:
    def __init__(self, *streams):
        self.streams = [s for s in streams if s is not None]
    def write(self, text):
        for s in self.streams:
            try:
                s.write(text); s.flush()
            except Exception:
                pass
    def flush(self):
        for s in self.streams:
            try:
                s.flush()
            except Exception:
                pass
    def isatty(self):
        return False

try:
    _log = open(LOG_FILE, "w", encoding="utf-8")
    sys.stdout = _Tee(sys.__stdout__, _log)
    sys.stderr = _Tee(sys.__stderr__, _log)
    import time as _time
    print(f"=== ai_server start {_time.strftime('%Y-%m-%d %H:%M:%S')} | python {sys.executable}")
except Exception:
    pass

import cv2
import socket
import struct
import numpy as np
import threading
import time
from ultralytics import YOLO
import ultralytics.utils.plotting
from collections import defaultdict, Counter

# เคลียร์ Path ของ OpenCV ที่อาจจะซ้ำซ้อน
sys.path = [p for p in sys.path if "c:\\opencv" not in p.lower()]

# ─── Replace YOLO Palette to Match C++ History ──────────────
ultralytics.utils.plotting.colors.palette = [
    (240, 128, 128),  # 0: Empty (LightCoral)
    (255, 165, 0),    # 1: Overripe (Orange)
    (50, 205, 50),    # 2: Ripe (LimeGreen)
    (192, 192, 192),  # 3: Rotten (Silver)
    (255, 215, 0),    # 4: Underripe (Gold)
    (0, 191, 255)     # 5: Unripe (DeepSkyBlue)
]

# ─── Model ────────────────────────────────────────────────────────
ONNX_PATH   = r"C:\projectsky\Final300.onnx"
ENGINE_PATH = r"C:\projectsky\Final300.engine"
MODEL_PATH  = ENGINE_PATH if os.path.exists(ENGINE_PATH) else ONNX_PATH
print(f"📦 โหลดโมเดล: {os.path.basename(MODEL_PATH)}")
model = YOLO(MODEL_PATH, task="segment")

# ใช้การ์ดจอ NVIDIA ถ้ามี (ต้องใช้ venv ที่ลง torch รุ่น CUDA + onnxruntime-gpu) ไม่มีก็ใช้ CPU
def _pick_device():
    if FORCE_CPU:
        return "cpu"
    try:
        import torch
        if torch.cuda.is_available() and torch.cuda.device_count() > 0:
            try:
                import onnxruntime
                onnxruntime.preload_dlls()  # โหลด CUDA/cuDNN ที่มากับ pip (ไม่ต้องลง CUDA Toolkit แยก)
            except Exception:
                pass
            return 0
    except Exception:
        pass
    return "cpu"

DEVICE = _pick_device()

def _warm_up(dev):
    # ใช้ model.track ด้วยค่าเดียวกับตอนทำงานจริง โมเดลจะได้ไม่ถูกโหลดซ้ำตอนเฟรมแรก
    # (ภาพดำ -> ไม่มีวัตถุ -> ไม่สร้าง track ใดๆ ไม่กระทบยอดนับ)
    model.track(np.zeros((480, 640, 3), np.uint8), imgsz=640, device=dev, tracker="botsort.yaml",
                persist=True, conf=0.4, iou=0.5, verbose=False)

# อุ่นเครื่อง: รันโมเดล 1 ครั้งก่อนต่อกับ C++ (ครั้งแรกช้าเพราะต้องเตรียม GPU/หน่วยความจำ)
# ถ้า GPU มีปัญหา ให้ถอยไปใช้ CPU เอง แทนที่จะล้มกลางเดโม
_t = time.time()
try:
    _warm_up(DEVICE)
except Exception as e:
    if DEVICE == "cpu":
        raise
    print(f"⚠️ ใช้ GPU ไม่ได้ ({e}) -> เปลี่ยนไปใช้ CPU")
    DEVICE = "cpu"
    model.predictor = None
    _warm_up(DEVICE)
_dev_name = "CPU"
if DEVICE != "cpu":
    import torch
    _dev_name = "GPU " + torch.cuda.get_device_name(0)
    try:  # onnxruntime ถอยไปใช้ CPU เงียบๆ ได้ถ้าโหลด CUDA ไม่สำเร็จ -> เช็กให้เห็นใน log
        _providers = model.predictor.model.session.get_providers()
        if "CUDAExecutionProvider" not in _providers:
            _dev_name += f" (แต่ onnxruntime ใช้ {_providers[0]} แทน!)"
    except Exception:
        pass
print(f"✅ โมเดลพร้อม (device={_dev_name}, warm-up {time.time() - _t:.1f}s)")

camera_config_file  = r"C:\projectsky\camera_source.txt"
display_config_file = r"C:\projectsky\display_config.txt"
camera_source = 0

if os.path.exists(camera_config_file):
    with open(camera_config_file, "r", encoding="utf-8") as f:
        val = f.read().strip()
        # ⚡ แปลงเป็น int ถ้าเป็นตัวเลข (สำหรับกล้อง webcam 0, 1, 2...)
        try:
            camera_source = int(val)
        except ValueError:
            # ถ้าไม่ใช่ตัวเลข เก็บเป็น string (IP Camera หรือไฟล์วิดีโอ)
            camera_source = val if val else 0
print(f"📌 Camera source: {camera_source}")

# ─── Socket ────────────────────────────────────────────────────────
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

show_box = True
show_mask = True
last_cfg_mtime = 0
frame_counter = 0

# ⚡ Track Manager (แก้ปัญหานับขึ้นลง ล็อคคลาส และเก็บยอดสะสมถาวร)
# กติกานับ: ทะลายต้องถูกเห็น "ต่อเนื่อง" อย่างน้อย DWELL_SECONDS วินาที จึงจะถูกนับ (+1 ครั้งเดียวต่อ track ID)
#   - ตัวที่โผล่แวบเดียว / ตรวจจับผิดชั่วคราว จะไม่ถูกนับ
#   - วัดเป็น "วินาที" ไม่ใช่จำนวนเฟรม ผลจึงไม่ขึ้นกับความเร็วเครื่อง/FPS
#     (ไฟล์วิดีโอใช้เวลาในวิดีโอ, กล้องสดใช้เวลาจริง)
#   - หายไปไม่เกิน GAP_SECONDS (เช่น ถูกบังชั่วครู่) ยังนับว่าต่อเนื่อง
# ค่า 1.0 วินาทีมาจากการทดสอบกับคลิปจริง: เดิม (5 เฟรม) นับเกินจริง 2.5-4 เท่าในคลิปที่เดินถ่ายรอบกอง,
# 1.0 วินาทีได้ใกล้ยอดจริงที่สุด, 1.5 วินาทีขึ้นไปเริ่มนับขาด
DWELL_SECONDS = 1.0
GAP_SECONDS = 0.3

class TrackManager:
    def __init__(self, dwell_s=DWELL_SECONDS, gap_s=GAP_SECONDS):
        self.dwell_s, self.gap_s = dwell_s, gap_s
        self.first_seen = {}                 # {track_id: เวลาที่เริ่มเห็นต่อเนื่องรอบนี้}
        self.last_seen = {}                  # {track_id: เวลาที่เห็นล่าสุด}
        self.votes = defaultdict(list)       # {track_id: [class ที่ทายในแต่ละเฟรม]}
        self.locked_tracks = {}              # {track_id: final_class}
        self.total_counts = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0}  # ยอดสะสมรวม ไม่มีวันลด

    def update_and_get_class(self, track_id, cls, now):
        # 1. ถ้า ID นี้ถูกล็อคคลาสและบวกยอดไปแล้ว ให้คืนค่าคลาสเดิมตลอดกาล (กันคลาสกระโดด)
        if track_id in self.locked_tracks:
            return self.locked_tracks[track_id]

        # 2. หายไปนานเกิน -> เริ่มจับเวลาใหม่
        last = self.last_seen.get(track_id)
        if last is None or now - last > self.gap_s:
            self.first_seen[track_id] = now
            self.votes[track_id] = []
        self.last_seen[track_id] = now
        self.votes[track_id].append(cls)
        stable_cls = Counter(self.votes[track_id]).most_common(1)[0][0]  # โหวตตลอดช่วงที่เห็น

        # 3. อยู่ครบเวลา -> ฟันธง ล็อคคลาส และบวกยอดสะสม (+1 แค่ครั้งเดียวต่อ 1 Track ID)
        if now - self.first_seen[track_id] >= self.dwell_s - 1e-6:
            self.locked_tracks[track_id] = stable_cls
            self.total_counts[stable_cls] += 1
            self._forget(track_id)

        # 4. ยังไม่ครบเวลา: แสดงคลาสจากการโหวตไปก่อน แต่ "ยังไม่บวกยอด"
        return stable_cls

    def cleanup_old_tracks(self, now):
        # ลบประวัติของตัวที่ยังไม่ทันถูกนับแล้วหายไปนานเกิน GAP_SECONDS
        for tid in [t for t, last in self.last_seen.items() if now - last > self.gap_s]:
            self._forget(tid)

    def _forget(self, tid):
        self.first_seen.pop(tid, None); self.last_seen.pop(tid, None); self.votes.pop(tid, None)

# สร้าง Track Manager ตัวเดียวใช้ตลอดทั้งโปรแกรม
track_manager = TrackManager()

def refresh_display_config():
    global show_box, show_mask, last_cfg_mtime
    try:
        if os.path.exists(display_config_file):
            mtime = os.path.getmtime(display_config_file)
            if mtime > last_cfg_mtime:
                with open(display_config_file, "r", encoding="utf-8") as f:
                    vals = f.read().strip().split(",")
                    if len(vals) >= 2:
                        show_box  = (vals[0] == "1")
                        show_mask = (vals[1] == "1")
                last_cfg_mtime = mtime
    except Exception:
        pass

def process_and_send(frame, now):
    # now = เวลาของเฟรมนี้ (วินาที): ไฟล์วิดีโอ = เวลาในวิดีโอ, กล้องสด = เวลาจริง
    refresh_display_config()
    global frame_counter

    results = model.track(
        frame, 
        verbose=False, 
        imgsz=640, 
        device=DEVICE,
        tracker="botsort.yaml",
        persist=True,
        conf=0.4,
        iou=0.5
    )

    annotated = results[0].plot(boxes=show_box, masks=show_mask, color_mode="class")

    str_lines = []
    active_track_ids = []

    if results[0].boxes is not None and len(results[0].boxes) > 0:
        masks_xy = results[0].masks.xy if results[0].masks is not None else []

        for i, box in enumerate(results[0].boxes):
            b    = box.xyxy[0].cpu().numpy().tolist()
            c    = int(box.cls.cpu().numpy()[0])
            conf = float(box.conf.cpu().numpy()[0]) if box.conf is not None else 0.99

            track_id = int(box.id.cpu().numpy()[0]) if box.id is not None else -1
            
            if track_id != -1:
                active_track_ids.append(track_id)
                # จัดการการล็อคคลาสและนับยอดสะสม
                stable_cls = track_manager.update_and_get_class(track_id, c, now)
            else:
                stable_cls = c

            x1, y1, x2, y2 = int(b[0]), int(b[1]), int(b[2]), int(b[3])

            pts_str = ""
            if i < len(masks_xy) and len(masks_xy[i]) > 0:
                contour = masks_xy[i]
                step = max(1, len(contour) // 20) 
                sampled_pts = contour[::step]
                coords = [f"{int(pt[0])},{int(pt[1])}" for pt in sampled_pts]
                pts_str = ",".join(coords)
            else:
                pts_str = f"{x1},{y1},{x2},{y1},{x2},{y2},{x1},{y2}"

            str_lines.append(f"{track_id};{stable_cls};{conf:.2f};{x1},{y1},{x2},{y2};{pts_str}")

    # ทำความสะอาด ID เก่าที่หลุดเฟรมไป (ไม่กระทบยอด Total)
    track_manager.cleanup_old_tracks(now)

    # ⚡⚡⚡ ดึงยอดสะสมรวม (Cumulative) ที่บวกไว้แบบถาวร ⚡⚡⚡
    tc = track_manager.total_counts
    summary = f"{tc[2]}|{tc[4]}|{tc[1]}|{tc[5]}|{tc[0]}|{tc[3]}"

    box_bytes = "\n".join(str_lines).encode('utf-8')

    _, buf = cv2.imencode('.jpg', annotated, [int(cv2.IMWRITE_JPEG_QUALITY), 55])
    img_bytes = buf.tobytes()

    frame_counter += 1
    # ส่งภาพดิบทุก 10 เฟรม (เริ่มที่เฟรมแรก) ฝั่ง C++ จะเก็บกรอบของ "เฟรมเดียวกัน" คู่กับภาพดิบนี้ไว้บันทึกประวัติ
    if frame_counter % 10 == 1:
        _, buf_raw = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 60])
        raw_bytes = buf_raw.tobytes()
    else:
        raw_bytes = b""

    text_bytes = summary.encode('utf-8')

    def send(data):
        client_socket.sendall(struct.pack('<I', len(data)))
        if data:
            client_socket.sendall(data)

    send(img_bytes)
    send(raw_bytes)
    send(text_bytes)
    send(box_bytes)


class VirtualWebcam:
    """แหล่งภาพ 2 แบบ
    - กล้องสด (webcam / IP / stream): อ่านใน thread แยก เก็บแค่เฟรมล่าสุด (ภาพไม่ดีเลย์)
    - ไฟล์วิดีโอ: อ่านตามลำดับแบบ "ข้ามเฟรมตายตัว" ให้เหลือ ~VIDEO_PROCESS_FPS เฟรม/วินาทีของวิดีโอ
      เดิมข้ามเฟรมตามความเร็วเครื่อง ยอดนับจึงเปลี่ยนไปตามเครื่อง/ทุกครั้งที่รัน ตอนนี้ได้ผลเท่ากันทุกครั้ง
      (ถ้าเครื่องช้ากว่านี้ วิดีโอจะเล่นช้าลงแต่ยอดนับไม่เปลี่ยน)
    """
    VIDEO_PROCESS_FPS = 10  # ใกล้กับความเร็ว CPU เดิม (~11 fps) ที่ทีมใช้ทดสอบระบบนับ
    LIVE_PREFIXES = ("http://", "https://", "rtsp://", "rtmp://", "udp://", "tcp://")

    def __init__(self, src):
        self.src  = src
        self.is_video = isinstance(src, str) and not src.lower().startswith(self.LIVE_PREFIXES)
        self.cap = cv2.VideoCapture(src)
        self.latest  = None
        self.timestamp = 0.0
        self.running = self.cap.isOpened()
        if not self.running:
            print(f"❌ เปิดแหล่งภาพไม่ได้: {src}")
            return

        if self.is_video:
            fps = self.cap.get(cv2.CAP_PROP_FPS)
            fps = fps if 1 < fps < 240 else 30
            self.step = max(1, round(fps / self.VIDEO_PROCESS_FPS))
            print(f"🎞️ ไฟล์วิดีโอ {fps:.0f} fps -> ประมวลผลทุก {self.step} เฟรม")
        else:
            self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
            threading.Thread(target=self._loop, daemon=True).start()

    def _loop(self):  # กล้องสดเท่านั้น
        while self.running and self.cap.isOpened():
            ret = self.cap.grab()
            if not ret:
                time.sleep(0.01)
                continue

            ok, frame = self.cap.retrieve()
            if ok:
                self.latest = frame

    def read(self):
        if self.is_video:
            for _ in range(self.step - 1):  # ข้ามเฟรมแบบไม่ถอดรหัสภาพ (เร็ว)
                if not self.cap.grab():
                    break
            ok, frame = self.cap.read()
            if not ok:
                self.running = False  # วิดีโอจบ
                return False, None
            self.timestamp = self.cap.get(cv2.CAP_PROP_POS_MSEC) / 1000.0  # เวลาในวิดีโอ
            return True, frame
        self.timestamp = time.monotonic()
        if self.latest is not None:
            f = self.latest
            self.latest = None
            return True, f
        return False, None

    def release(self):
        self.running = False
        self.cap.release()

if __name__ == '__main__':
    # รอ C++ เปิดพอร์ตแบบไม่กิน CPU และเลิกรอเองถ้านานเกิน (กัน Process ผีค้างเมื่อปิดโปรแกรม C++ ไปก่อน)
    CONNECT_TIMEOUT_S = 30
    print("⏳ รอเชื่อมต่อ C++...")
    deadline = time.time() + CONNECT_TIMEOUT_S
    while True:
        try:
            client_socket.connect(('127.0.0.1', 8888))
            print("✅ เชื่อมต่อสำเร็จ!")
            break
        except OSError:
            client_socket.close()
            if time.time() > deadline:
                print(f"❌ ไม่มี C++ รอรับที่พอร์ต 8888 ภายใน {CONNECT_TIMEOUT_S} วินาที -> ปิดตัวเอง")
                sys.exit(1)
            time.sleep(0.2)
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        cam = VirtualWebcam(camera_source)
        time.sleep(1)

        print("🎥 เริ่มประมวลผล (BoTSORT + Lock Class & Cumulative Counter)")
        while cam.running:
            ok, frame = cam.read()
            if not ok or frame is None:
                time.sleep(0.005)
                continue

            # ตั้งใจ "บีบ" เป็น 640x480 ตามเดิม: ทดสอบแล้วว่าถ้าคงสัดส่วนภาพ (เช่น วิดีโอแนวตั้ง)
            # โมเดลนี้แยกคลาสเพี้ยน (ทะลายสุกกลายเป็น unripe/underripe) - น่าจะเพราะชุดฝึกถูกยืดภาพมา
            frame = cv2.resize(frame, (640, 480))
            process_and_send(frame, cam.timestamp)
            if frame_counter == 1:
                _t_first = time.time()
            elif frame_counter % 100 == 0:
                print(f"… {frame_counter} เฟรม, {99 / (time.time() - _t_first):.1f} fps")
                _t_first = time.time()

        cam.release()
    except Exception as e:
        print(f"❌ Error: {e}")
        import traceback
        traceback.print_exc()
    finally:
        time.sleep(1.5)
        client_socket.close()
        tc = track_manager.total_counts
        print(f"📊 ประมวลผล {frame_counter} เฟรม | ยอดสะสม ripe={tc[2]} underripe={tc[4]} overripe={tc[1]} "
              f"unripe={tc[5]} empty={tc[0]} rotten={tc[3]}")
        print("ปิดระบบเรียบร้อย")