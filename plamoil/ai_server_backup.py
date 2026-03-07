import sys
import os
import cv2
import socket
import struct
import numpy as np
from ultralytics import YOLO

sys.path = [p for p in sys.path if "c:\\opencv" not in p.lower()]

import threading
import time
import torch

# ─── GPU Detection ───────────────────────────────────────────────────────────
DEVICE = "cuda:0" if torch.cuda.is_available() else "cpu"
if DEVICE == "cuda:0":
    gpu_name = torch.cuda.get_device_name(0)
    gpu_mem  = torch.cuda.get_device_properties(0).total_memory // 1024**3
    print(f"🚀 GPU: {gpu_name} ({gpu_mem} GB VRAM)")
else:
    print("💻 GPU ไม่พบ → ใช้ CPU")

# ─── Model Loading (TRT Engine → ONNX fallback) ───────────────────────────────
ONNX_PATH   = r"C:\projectsky\Finalmak2Model.onnx"
ENGINE_PATH = r"C:\projectsky\Finalmak2Model.engine"

MODEL_PATH = ENGINE_PATH if os.path.exists(ENGINE_PATH) else ONNX_PATH
print(f"📦 โหลดโมเดล: {os.path.basename(MODEL_PATH)}")
model = YOLO(MODEL_PATH, task="segment")

# ─── State ────────────────────────────────────────────────────────────────────
seen_ids = {0: set(), 1: set(), 2: set(), 3: set(), 4: set(), 5: set()}

camera_config_file  = r"C:\projectsky\camera_source.txt"
display_config_file = r"C:\projectsky\display_config.txt"
camera_source = 0

if os.path.exists(camera_config_file):
    with open(camera_config_file, "r", encoding="utf-8") as f:
        val = f.read().strip()
        camera_source = 0 if val == "0" else (val if val else 0)

print(f"📌 Camera source: {camera_source}")

# ─── Socket ───────────────────────────────────────────────────────────────────
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("⏳ รอเชื่อมต่อ C++...")
while True:
    try:
        client_socket.connect(('127.0.0.1', 8888))
        print("✅ เชื่อมต่อสำเร็จ!")
        break
    except ConnectionRefusedError:
        pass

# ─── Display Config Cache ─────────────────────────────────────────────────────
show_box       = True
show_mask      = True
last_cfg_mtime = 0
frame_counter  = 0

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

# ─── AI Process + Send ────────────────────────────────────────────────────────
def process_and_send(frame):
    global frame_counter

    refresh_display_config()

    # Run YOLO inference (imgsz=384 = 2-3x faster than default 640)
    results = model.track(frame, persist=True, tracker="bytetrack.yaml",
                          verbose=False, imgsz=384)
    annotated = results[0].plot(boxes=show_box, masks=show_mask)

    # Update cumulative detection counts
    if results[0].boxes is not None and results[0].boxes.id is not None:
        ids = results[0].boxes.id.cpu().numpy().astype(int)
        cls = results[0].boxes.cls.cpu().numpy().astype(int)
        for obj_id, obj_cls in zip(ids, cls):
            seen_ids[obj_cls].add(obj_id)

    summary = (f"{len(seen_ids[2])}|{len(seen_ids[4])}|{len(seen_ids[1])}"
               f"|{len(seen_ids[5])}|{len(seen_ids[0])}|{len(seen_ids[3])}")

    # Build bounding box string
    str_lines = []
    if results[0].boxes is not None and len(results[0].boxes) > 0:
        for box in results[0].boxes:
            b    = box.xyxy[0].cpu().numpy().tolist()
            c    = int(box.cls.cpu().numpy()[0])
            conf = float(box.conf.cpu().numpy()[0]) if box.conf is not None else 0.99
            oid  = int(box.id.cpu().numpy()[0]) if box.id is not None else 0
            str_lines.append(f"{oid};{c};{conf:.2f};{int(b[0])},{int(b[1])},{int(b[2])},{int(b[3])};")
    box_bytes = "\n".join(str_lines).encode('utf-8')

    # Encode annotated frame → JPEG
    _, buf = cv2.imencode('.jpg', annotated, [int(cv2.IMWRITE_JPEG_QUALITY), 55])
    img_bytes = buf.tobytes()

    # Raw frame every 10 frames (only needed for history save)
    frame_counter += 1
    if frame_counter % 10 == 0:
        _, buf_raw = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 60])
        raw_bytes = buf_raw.tobytes()
    else:
        raw_bytes = b""

    text_bytes = summary.encode('utf-8')

    # Send 4 packets to C++
    client_socket.sendall(struct.pack('<I', len(img_bytes)));  client_socket.sendall(img_bytes)
    client_socket.sendall(struct.pack('<I', len(raw_bytes)));
    if raw_bytes: client_socket.sendall(raw_bytes)
    client_socket.sendall(struct.pack('<I', len(text_bytes))); client_socket.sendall(text_bytes)
    client_socket.sendall(struct.pack('<I', len(box_bytes)));
    if box_bytes: client_socket.sendall(box_bytes)

# ─── VirtualWebcam ────────────────────────────────────────────────────────────
class VirtualWebcam:
    """
    Single-slot frame cache:
    - Background thread continuously grabs from camera (drains OS buffer → no lag)
    - Main thread reads the latest frame whenever AI is ready
    """
    def __init__(self, src):
        self.src = src
        self.cap = cv2.VideoCapture(self.src)
        self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

        self.is_video_file = isinstance(self.src, str) and not self.src.startswith("http")
        fps = self.cap.get(cv2.CAP_PROP_FPS)
        self.fps         = fps if 1 < fps < 120 else 30
        self.frame_delay = 1.0 / self.fps

        self.latest_frame = None
        self.running      = True

        t = threading.Thread(target=self._capture_loop, daemon=True)
        t.start()

    def _capture_loop(self):
        while self.running and self.cap.isOpened():
            t0  = time.time()
            ret = self.cap.grab()
            if not ret:
                if self.is_video_file:
                    self.running = False
                else:
                    time.sleep(0.01)
                continue

            # Always retrieve the latest decoded frame (overwrites stale copy)
            ok, frame = self.cap.retrieve()
            if ok:
                self.latest_frame = frame

            # For video files: pace to native FPS so it plays in real-time
            if self.is_video_file:
                wait = self.frame_delay - (time.time() - t0)
                if wait > 0:
                    time.sleep(wait)

    def read(self):
        if self.latest_frame is not None:
            f = self.latest_frame
            self.latest_frame = None
            return True, f
        return False, None

    def release(self):
        self.running = False
        self.cap.release()

# ─── Main Loop ────────────────────────────────────────────────────────────────
try:
    cam = VirtualWebcam(camera_source)
    time.sleep(1)  # Warm up buffer

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
finally:
    client_socket.close()
    print("ปิดระบบเรียบร้อย")
