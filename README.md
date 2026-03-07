# 🌴 Smart Palm App (AI Palm Oil Ripeness Detector)

โปรแกรมวิเคราะห์และนับจำนวนผลปาล์มน้ำมันแบบเรียลไทม์โดยใช้ AI (YOLO) ผสานกับหน้าต่างโปรแกรม C++ (Windows Forms) เพื่อแสดงผล ประเมินคุณภาพ และออกใบเสร็จรับเงินสำหรับลานเทปาล์ม

---

## ⚙️ สิ่งที่ต้องเตรียมก่อนเริ่มงาน (Prerequisites)

เนื่องจากโมเดล AI มีขนาดใหญ่เกินกว่าที่ GitHub จะรองรับได้ คุณจำเป็นต้องดาวน์โหลดโมเดลมาใส่ด้วยตัวเองก่อนรันโปรแกรม

### 1. โคลนโปรเจคนี้ลงเครื่อง
```bash
git clone https://github.com/Peeranatz/SmartPalmApp.git
cd SmartPalmApp
```

### 2. ดาวน์โหลดโมเดล AI (ONNX)
1. ดาวน์โหลดไฟล์โมเดลชื่อ `Finalmak2Model.onnx` จากลิงก์นี้: 
👉 **[แปะลิงก์ Google Drive ของคุณแจ็คตรงนี้]**
2. นำไฟล์ `Finalmak2Model.onnx` ที่โหลดเสร็จแล้ว ไปวางไว้ที่โฟลเดอร์ `C:\projectsky\` (หรือแก้ที่อยู่พาทในไฟล์ `plamoil/ai_server.py` บรรทัด `ONNX_PATH` ให้ตรงกับพาทที่คุณวางไฟล์)

---

## 🐍 การติดตั้งและรันฝั่งหลังบ้าน (Python AI Backend)

โปรแกรมเราใช้ Python ทำหน้าที่เป็นสมอง AI ในการวิเคราะห์ภาพวิดีโอ

1. ติดตั้งไลบรารีที่จำเป็น (แนะนำให้รันใน Command Prompt):
```bash
pip install ultralytics opencv-python onnxruntime numpy
```

2. การรันเซิร์ฟเวอร์ AI **(ไม่ต้องรันด้วยมือ!)**:
- เมื่อคุณกดปุ่ม **"Start Video"** ในตัวโปรแกรมหลักฝั่ง C++ มันจะทำการปลุก (Launch) `ai_server.py` ขึ้นมาทำงานเป็นเบื้องหลังโดยอัตโนมัติ

---

## 🖥️ การเปิดโปรแกรมหน้าบ้าน (C++ Windows Forms)

1. ต้องมีโปรแกรม **Visual Studio 2022** (พร้อมติดตั้ง Workload "Desktop development with C++" และ "C++/CLI support").
2. ดับเบิลคลิกไฟล์ **`plamoil.sln`** เพื่อเปิดโปรเจคใน Visual Studio
3. ด้านบนของ Visual Studio ให้ตั้งค่า Build เป็น:
   - **Configuration:** `Debug` หรือ `Release`
   - **Platform:** `x64` (สำคัญมาก! OpenCV ต้องใช้ x64)
4. กดปุ่ม ▶️ **Local Windows Debugger** (หรือกด F5) เพื่อสตาร์ทโปรแกรม!

---

## 💡 โครงสร้างการส่งข้อมูล (Architecture)
- **Frontend (UI):** เขียนด้วย C++ / CLI (Windows Forms) ทำหน้าที่แสดงผลภาพกราฟและออกใบเสร็จ
- **Backend (AI):** เขียนด้วย Python (`ai_server.py`) ใช้โมเดล YOLO เซกเมนต์ผลปาล์มแล้วส่งภาพ/ข้อมูลผ่าน Local Socket (พอร์ต 8888) ยิงตรงเข้าหา C++ แบบเฟรมต่อเฟรม (Per-Frame Status) ทำให้ระบบรวดเร็ว เสถียร และไม่กินสเปคคอม!