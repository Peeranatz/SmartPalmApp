# C:\projectsky\ai_server.py - Fixed Version with BoTSORT & Lock Class Tracking

import os
os.environ["CUDA_VISIBLE_DEVICES"] = ""

import sys
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
print("✅ โมเดลพร้อม")

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
class TrackManager:
    def __init__(self, history_size=5):
        self.track_history = defaultdict(list)
        self.locked_tracks = {}  # {track_id: final_class}
        self.total_counts = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0} # ยอดสะสมรวม ไม่มีวันลด
        self.history_size = history_size
    
    def update_and_get_class(self, track_id, cls):
        # 1. ถ้า ID นี้ถูกล็อคคลาสและบวกยอดไปแล้ว ให้คืนค่าคลาสเดิมตลอดกาล (กันคลาสกระโดด)
        if track_id in self.locked_tracks:
            return self.locked_tracks[track_id]
        
        # 2. เก็บประวัติคลาสของเฟรมปัจจุบัน
        self.track_history[track_id].append(cls)
        
        # 3. ถ้าเจอ ID นี้ติดต่อกันครบจำนวนที่กำหนด -> ฟันธง
        if len(self.track_history[track_id]) >= self.history_size:
            counter = Counter(self.track_history[track_id])
            stable_cls = counter.most_common(1)[0][0]
            
            # ล็อคคลาส และ บวกยอดสะสม (+1 แค่ครั้งเดียวต่อ 1 Track ID)
            self.locked_tracks[track_id] = stable_cls
            self.total_counts[stable_cls] += 1
            
            # ลบประวัติชั่วคราวทิ้งได้เลย
            del self.track_history[track_id]
            return stable_cls
        
        # 4. ถ้าเพิ่งโผล่มา (ยังไม่ครบเกณฑ์) ให้โหวตจากข้อมูลที่มีไปก่อน แต่ "ยังไม่บวกยอด"
        counter = Counter(self.track_history[track_id])
        return counter.most_common(1)[0][0]

    def cleanup_old_tracks(self, active_track_ids):
        # ลบประวัติเฉพาะตัวที่ยังไม่ทันล็อคคลาสแล้วหลุดจอไปก่อน
        old_ids = set(self.track_history.keys()) - set(active_track_ids)
        for tid in old_ids:
            del self.track_history[tid]

# สร้าง Track Manager ตัวเดียวใช้ตลอดทั้งโปรแกรม
track_manager = TrackManager(history_size=5)

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

def process_and_send(frame):
    refresh_display_config()
    global frame_counter

    results = model.track(
        frame, 
        verbose=False, 
        imgsz=640, 
        device="cpu",
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
                stable_cls = track_manager.update_and_get_class(track_id, c)
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
    track_manager.cleanup_old_tracks(active_track_ids)

    # ⚡⚡⚡ ดึงยอดสะสมรวม (Cumulative) ที่บวกไว้แบบถาวร ⚡⚡⚡
    tc = track_manager.total_counts
    summary = f"{tc[2]}|{tc[4]}|{tc[1]}|{tc[5]}|{tc[0]}|{tc[3]}"

    box_bytes = "\n".join(str_lines).encode('utf-8')

    _, buf = cv2.imencode('.jpg', annotated, [int(cv2.IMWRITE_JPEG_QUALITY), 55])
    img_bytes = buf.tobytes()

    frame_counter += 1
    if frame_counter % 10 == 0:
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
    def __init__(self, src):
        self.src  = src
        self.is_video = isinstance(src, str) and not src.startswith("http")
        self.cap = cv2.VideoCapture(src)
        if not self.is_video:
            self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

        fps = self.cap.get(cv2.CAP_PROP_FPS)
        self.delay = 1.0 / (fps if 1 < fps < 120 else 30)

        self.latest  = None
        self.running = True
        threading.Thread(target=self._loop, daemon=True).start()

    def _loop(self):
        while self.running and self.cap.isOpened():
            t0  = time.time()
            ret = self.cap.grab()
            if not ret:
                if self.is_video:
                    self.running = False
                else:
                    time.sleep(0.01)
                continue

            ok, frame = self.cap.retrieve()
            if ok:
                self.latest = frame

            if self.is_video:
                wait = self.delay - (time.time() - t0)
                if wait > 0:
                    time.sleep(wait)

    def read(self):
        if self.latest is not None:
            f = self.latest
            self.latest = None
            return True, f
        return False, None

    def release(self):
        self.running = False
        self.cap.release()

if __name__ == '__main__':
    print("⏳ รอเชื่อมต่อ C++...")
    while True:
        try:
            client_socket.connect(('127.0.0.1', 8888))
            print("✅ เชื่อมต่อสำเร็จ!")
            break
        except ConnectionRefusedError:
            pass
            
    try:
        cam = VirtualWebcam(camera_source)
        time.sleep(1)

        print("🎥 เริ่มประมวลผล (BoTSORT + Lock Class & Cumulative Counter)")
        while cam.running:
            ok, frame = cam.read()
            if not ok or frame is None:
                time.sleep(0.005)
                continue

            frame = cv2.resize(frame, (640, 480))
            process_and_send(frame)

        cam.release()
    except Exception as e:
        print(f"❌ Error: {e}")
        import traceback
        traceback.print_exc()
    finally:
        time.sleep(1.5)
        client_socket.close()
        print("ปิดระบบเรียบร้อย")