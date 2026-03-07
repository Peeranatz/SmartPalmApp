#pragma once

namespace plamoil {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class SettingsForm : public Form {
    public:
        // ตัวแปรสำหรับส่งค่ากลับไปให้หน้าต่างหลัก
        double BasePrice, DedEmpty, DedOverripe, DedRipe, DedRotten, DedUnderripe, DedUnripe;

    private:
        TextBox^ txtBP; TextBox^ txtEmpty; TextBox^ txtOverripe;
        TextBox^ txtRipe; TextBox^ txtRotten; TextBox^ txtUnderripe; TextBox^ txtUnripe;

    public:
        SettingsForm(double bp, double e, double o, double r, double rot, double u, double un) {
            // ตั้งค่าหน้าต่างป๊อปอัป
            this->Text = L"ตั้งค่าสูตรราคารับซื้อ ⚙️";
            this->Size = Drawing::Size(400, 520);
            this->StartPosition = FormStartPosition::CenterParent; // ให้เด้งตรงกลางจอ
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->BackColor = Color::White;

            // หัวข้อราคาฐาน
            Label^ lbl1 = gcnew Label(); lbl1->Text = L"ราคาปาล์มฐาน (บาท/กก.)"; lbl1->Location = Point(20, 20); lbl1->AutoSize = true; lbl1->Font = gcnew Drawing::Font(L"Tahoma", 10, FontStyle::Bold); this->Controls->Add(lbl1);
            txtBP = gcnew TextBox(); txtBP->Text = bp.ToString(); txtBP->Location = Point(20, 45); txtBP->Width = 340; txtBP->Font = gcnew Drawing::Font(L"Tahoma", 14, FontStyle::Bold); txtBP->ForeColor = Color::MediumSeaGreen; this->Controls->Add(txtBP);

            Label^ lbl2 = gcnew Label(); lbl2->Text = L"อัตราหักเงิน (บาท/กก.) *ใส่ตัวเลขเช่น 0.5"; lbl2->Location = Point(20, 100); lbl2->AutoSize = true; lbl2->Font = gcnew Drawing::Font(L"Tahoma", 10, FontStyle::Bold); this->Controls->Add(lbl2);

            // สร้างช่องกรอกอัตราหักเงิน 6 คลาส
            int startY = 130;
            txtEmpty = CreateRow(L"ทะลายเปล่า (Empty Bunch):", e, startY); startY += 45;
            txtOverripe = CreateRow(L"สุกเกิน (Overripe):", o, startY); startY += 45;
            txtRipe = CreateRow(L"สุกพอดี (Ripe):", r, startY); startY += 45;
            txtRotten = CreateRow(L"เน่า (Rotten):", rot, startY); startY += 45;
            txtUnderripe = CreateRow(L"ห่าม (Underripe):", u, startY); startY += 45;
            txtUnripe = CreateRow(L"ดิบ (Unripe):", un, startY); startY += 45;

            // ปุ่มบันทึก
            Button^ btnSave = gcnew Button();
            btnSave->Text = L"บันทึกการตั้งค่า";
            btnSave->Location = Point(20, startY + 10);
            btnSave->Size = Drawing::Size(340, 45);
            btnSave->BackColor = Color::MediumSeaGreen;
            btnSave->ForeColor = Color::White;
            btnSave->FlatStyle = FlatStyle::Flat;
            btnSave->Font = gcnew Drawing::Font(L"Tahoma", 12, FontStyle::Bold);
            btnSave->Click += gcnew EventHandler(this, &SettingsForm::OnSaveClick);
            this->Controls->Add(btnSave);
        }

    private:
        // ฟังก์ชันช่วยวาดกล่องข้อความ
        TextBox^ CreateRow(String^ labelText, double val, int y) {
            Label^ lbl = gcnew Label(); lbl->Text = labelText; lbl->Location = Point(20, y + 8); lbl->AutoSize = true; lbl->Font = gcnew Drawing::Font(L"Tahoma", 10); this->Controls->Add(lbl);
            TextBox^ txt = gcnew TextBox(); txt->Text = val.ToString(); txt->Location = Point(260, y); txt->Width = 100; txt->Font = gcnew Drawing::Font(L"Tahoma", 12); txt->TextAlign = HorizontalAlignment::Right; this->Controls->Add(txt);
            return txt;
        }

        // เมื่อกดปุ่มบันทึก
        void OnSaveClick(Object^ sender, EventArgs^ e) {
            try {
                BasePrice = Convert::ToDouble(txtBP->Text);
                DedEmpty = Convert::ToDouble(txtEmpty->Text);
                DedOverripe = Convert::ToDouble(txtOverripe->Text);
                DedRipe = Convert::ToDouble(txtRipe->Text);
                DedRotten = Convert::ToDouble(txtRotten->Text);
                DedUnderripe = Convert::ToDouble(txtUnderripe->Text);
                DedUnripe = Convert::ToDouble(txtUnripe->Text);
                this->DialogResult = System::Windows::Forms::DialogResult::OK; // ส่งสัญญาณว่าบันทึกสำเร็จ
                this->Close();
            }
            catch (...) {
                MessageBox::Show(L"กรุณากรอกตัวเลขให้ถูกต้อง", L"ข้อผิดพลาด", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    };
}