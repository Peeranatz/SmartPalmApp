#pragma once

namespace plamoil {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for HistoryForm
	/// </summary>
	public ref class HistoryForm : public System::Windows::Forms::Form
	{
	public:
		HistoryForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~HistoryForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataGridView^ gridHistory;
	private: System::Windows::Forms::PictureBox^ picHisImage;
	private: System::Windows::Forms::PictureBox^ picHisSlip;


	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chartHis;
	private: System::Windows::Forms::Button^ btnSaveChanges;
	private: System::Windows::Forms::Button^ btnDeleteSelected;
	private: System::Windows::Forms::Button^ btnDeleteAll;
	private: System::Windows::Forms::DateTimePicker^ dtpStartDate;
	private: System::Windows::Forms::DateTimePicker^ dtpEndDate;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ btnFilter;
	private: System::Windows::Forms::Button^ btnResetFilter;
	private: System::Windows::Forms::Label^ lblSumRipe;
	private: System::Windows::Forms::Label^ lblSumUnderripe;
	private: System::Windows::Forms::Label^ lblSumOverripe;
	private: System::Windows::Forms::Label^ lblSumUnripe;
	private: System::Windows::Forms::Label^ lblSumEmpty;
	private: System::Windows::Forms::Label^ lblSumRotten;
	private: System::Windows::Forms::Label^ lblSumTotal;







	protected:


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea4 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->gridHistory = (gcnew System::Windows::Forms::DataGridView());
			this->picHisImage = (gcnew System::Windows::Forms::PictureBox());
			this->picHisSlip = (gcnew System::Windows::Forms::PictureBox());
			this->chartHis = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->btnSaveChanges = (gcnew System::Windows::Forms::Button());
			this->btnDeleteSelected = (gcnew System::Windows::Forms::Button());
			this->btnDeleteAll = (gcnew System::Windows::Forms::Button());
			this->dtpStartDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->dtpEndDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnFilter = (gcnew System::Windows::Forms::Button());
			this->btnResetFilter = (gcnew System::Windows::Forms::Button());
			this->lblSumRipe = (gcnew System::Windows::Forms::Label());
			this->lblSumUnderripe = (gcnew System::Windows::Forms::Label());
			this->lblSumOverripe = (gcnew System::Windows::Forms::Label());
			this->lblSumUnripe = (gcnew System::Windows::Forms::Label());
			this->lblSumEmpty = (gcnew System::Windows::Forms::Label());
			this->lblSumRotten = (gcnew System::Windows::Forms::Label());
			this->lblSumTotal = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gridHistory))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picHisImage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picHisSlip))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartHis))->BeginInit();
			this->SuspendLayout();
			// 
			// gridHistory
			// 
			this->gridHistory->AllowUserToOrderColumns = true;
			this->gridHistory->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->gridHistory->Location = System::Drawing::Point(12, 90);
			this->gridHistory->Name = L"gridHistory";
			this->gridHistory->Size = System::Drawing::Size(391, 387);
			this->gridHistory->TabIndex = 0;
			this->gridHistory->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &HistoryForm::gridHistory_CellClick);
			// 
			// picHisImage
			// 
			this->picHisImage->BackColor = System::Drawing::Color::Black;
			this->picHisImage->Location = System::Drawing::Point(409, 12);
			this->picHisImage->Name = L"picHisImage";
			this->picHisImage->Size = System::Drawing::Size(514, 657);
			this->picHisImage->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->picHisImage->TabIndex = 1;
			this->picHisImage->TabStop = false;
			// 
			// picHisSlip
			// 
			this->picHisSlip->BackColor = System::Drawing::Color::White;
			this->picHisSlip->Location = System::Drawing::Point(929, 212);
			this->picHisSlip->Name = L"picHisSlip";
			this->picHisSlip->Size = System::Drawing::Size(323, 457);
			this->picHisSlip->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->picHisSlip->TabIndex = 2;
			this->picHisSlip->TabStop = false;
			// 
			// chartHis
			// 
			chartArea4->Name = L"ChartArea1";
			this->chartHis->ChartAreas->Add(chartArea4);
			legend4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(48)));
			legend4->ForeColor = System::Drawing::Color::White;
			legend4->Name = L"Legend1";
			this->chartHis->Legends->Add(legend4);
			this->chartHis->Location = System::Drawing::Point(929, 24);
			this->chartHis->Name = L"chartHis";
			series4->ChartArea = L"ChartArea1";
			series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Doughnut;
			series4->LabelForeColor = System::Drawing::Color::White;
			series4->Legend = L"Legend1";
			series4->Name = L"Series1";
			this->chartHis->Series->Add(series4);
			this->chartHis->Size = System::Drawing::Size(323, 182);
			this->chartHis->TabIndex = 3;
			this->chartHis->Text = L"chart1";
			this->chartHis->Click += gcnew System::EventHandler(this, &HistoryForm::chartHistory_Click);
			// 
			// btnSaveChanges
			// 
			this->btnSaveChanges->Location = System::Drawing::Point(328, 3);
			this->btnSaveChanges->Name = L"btnSaveChanges";
			this->btnSaveChanges->Size = System::Drawing::Size(75, 23);
			this->btnSaveChanges->TabIndex = 4;
			this->btnSaveChanges->Text = L"Save Edit";
			this->btnSaveChanges->UseVisualStyleBackColor = true;
			this->btnSaveChanges->Click += gcnew System::EventHandler(this, &HistoryForm::btnSaveChanges_Click);
			// 
			// btnDeleteSelected
			// 
			this->btnDeleteSelected->Location = System::Drawing::Point(328, 32);
			this->btnDeleteSelected->Name = L"btnDeleteSelected";
			this->btnDeleteSelected->Size = System::Drawing::Size(75, 23);
			this->btnDeleteSelected->TabIndex = 5;
			this->btnDeleteSelected->Text = L"Delete";
			this->btnDeleteSelected->UseVisualStyleBackColor = true;
			this->btnDeleteSelected->Click += gcnew System::EventHandler(this, &HistoryForm::btnDeleteSelected_Click);
			// 
			// btnDeleteAll
			// 
			this->btnDeleteAll->Location = System::Drawing::Point(328, 61);
			this->btnDeleteAll->Name = L"btnDeleteAll";
			this->btnDeleteAll->Size = System::Drawing::Size(75, 23);
			this->btnDeleteAll->TabIndex = 6;
			this->btnDeleteAll->Text = L"Clear All";
			this->btnDeleteAll->UseVisualStyleBackColor = true;
			this->btnDeleteAll->Click += gcnew System::EventHandler(this, &HistoryForm::btnDeleteAll_Click);
			// 
			// dtpStartDate
			// 
			this->dtpStartDate->CustomFormat = L"dd/MM/yyyy";
			this->dtpStartDate->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dtpStartDate->Location = System::Drawing::Point(55, 17);
			this->dtpStartDate->Name = L"dtpStartDate";
			this->dtpStartDate->Size = System::Drawing::Size(99, 20);
			this->dtpStartDate->TabIndex = 7;
			// 
			// dtpEndDate
			// 
			this->dtpEndDate->CustomFormat = L"dd/MM/yyyy";
			this->dtpEndDate->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dtpEndDate->Location = System::Drawing::Point(183, 17);
			this->dtpEndDate->Name = L"dtpEndDate";
			this->dtpEndDate->Size = System::Drawing::Size(98, 20);
			this->dtpEndDate->TabIndex = 8;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::SystemColors::ActiveCaption;
			this->label1->Location = System::Drawing::Point(18, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(37, 13);
			this->label1->TabIndex = 9;
			this->label1->Text = L"ตั้งแต่:";
			this->label1->Click += gcnew System::EventHandler(this, &HistoryForm::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::SystemColors::ActiveCaption;
			this->label2->Location = System::Drawing::Point(160, 17);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(22, 13);
			this->label2->TabIndex = 10;
			this->label2->Text = L"ถึง:";
			// 
			// btnFilter
			// 
			this->btnFilter->Location = System::Drawing::Point(21, 43);
			this->btnFilter->Name = L"btnFilter";
			this->btnFilter->Size = System::Drawing::Size(75, 23);
			this->btnFilter->TabIndex = 11;
			this->btnFilter->Text = L"🔍 ค้นหา";
			this->btnFilter->UseVisualStyleBackColor = true;
			this->btnFilter->Click += gcnew System::EventHandler(this, &HistoryForm::btnFilter_Click);
			// 
			// btnResetFilter
			// 
			this->btnResetFilter->Location = System::Drawing::Point(108, 43);
			this->btnResetFilter->Name = L"btnResetFilter";
			this->btnResetFilter->Size = System::Drawing::Size(97, 23);
			this->btnResetFilter->TabIndex = 12;
			this->btnResetFilter->Text = L"🔄 แสดงทั้งหมด";
			this->btnResetFilter->UseVisualStyleBackColor = true;
			this->btnResetFilter->Click += gcnew System::EventHandler(this, &HistoryForm::btnResetFilter_Click);
			// 
			// lblSumRipe
			// 
			this->lblSumRipe->AutoSize = true;
			this->lblSumRipe->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSumRipe->ForeColor = System::Drawing::Color::LimeGreen;
			this->lblSumRipe->Location = System::Drawing::Point(8, 491);
			this->lblSumRipe->Name = L"lblSumRipe";
			this->lblSumRipe->Size = System::Drawing::Size(68, 16);
			this->lblSumRipe->TabIndex = 13;
			this->lblSumRipe->Text = L"สุกพอดี: 0";
			// 
			// lblSumUnderripe
			// 
			this->lblSumUnderripe->AutoSize = true;
			this->lblSumUnderripe->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSumUnderripe->ForeColor = System::Drawing::Color::Gold;
			this->lblSumUnderripe->Location = System::Drawing::Point(8, 517);
			this->lblSumUnderripe->Name = L"lblSumUnderripe";
			this->lblSumUnderripe->Size = System::Drawing::Size(98, 16);
			this->lblSumUnderripe->TabIndex = 14;
			this->lblSumUnderripe->Text = L"ห่าม (กึ่งสุก): 0";
			this->lblSumUnderripe->Click += gcnew System::EventHandler(this, &HistoryForm::lblSumUnderripe_Click);
			// 
			// lblSumOverripe
			// 
			this->lblSumOverripe->AutoSize = true;
			this->lblSumOverripe->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSumOverripe->ForeColor = System::Drawing::Color::Orange;
			this->lblSumOverripe->Location = System::Drawing::Point(8, 545);
			this->lblSumOverripe->Name = L"lblSumOverripe";
			this->lblSumOverripe->Size = System::Drawing::Size(63, 16);
			this->lblSumOverripe->TabIndex = 15;
			this->lblSumOverripe->Text = L"สุกเกิน: 0";
			// 
			// lblSumUnripe
			// 
			this->lblSumUnripe->AutoSize = true;
			this->lblSumUnripe->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSumUnripe->ForeColor = System::Drawing::Color::MediumPurple;
			this->lblSumUnripe->Location = System::Drawing::Point(8, 570);
			this->lblSumUnripe->Name = L"lblSumUnripe";
			this->lblSumUnripe->Size = System::Drawing::Size(42, 16);
			this->lblSumUnripe->TabIndex = 16;
			this->lblSumUnripe->Text = L"ดิบ: 0";
			// 
			// lblSumEmpty
			// 
			this->lblSumEmpty->AutoSize = true;
			this->lblSumEmpty->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSumEmpty->ForeColor = System::Drawing::Color::LightCoral;
			this->lblSumEmpty->Location = System::Drawing::Point(8, 599);
			this->lblSumEmpty->Name = L"lblSumEmpty";
			this->lblSumEmpty->Size = System::Drawing::Size(91, 16);
			this->lblSumEmpty->TabIndex = 17;
			this->lblSumEmpty->Text = L"ทะลายเปล่า: 0";
			// 
			// lblSumRotten
			// 
			this->lblSumRotten->AutoSize = true;
			this->lblSumRotten->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSumRotten->ForeColor = System::Drawing::Color::Tan;
			this->lblSumRotten->Location = System::Drawing::Point(8, 626);
			this->lblSumRotten->Name = L"lblSumRotten";
			this->lblSumRotten->Size = System::Drawing::Size(44, 16);
			this->lblSumRotten->TabIndex = 18;
			this->lblSumRotten->Text = L"เน่า: 0";
			// 
			// lblSumTotal
			// 
			this->lblSumTotal->AutoSize = true;
			this->lblSumTotal->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblSumTotal->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->lblSumTotal->Location = System::Drawing::Point(8, 649);
			this->lblSumTotal->Name = L"lblSumTotal";
			this->lblSumTotal->Size = System::Drawing::Size(167, 19);
			this->lblSumTotal->TabIndex = 19;
			this->lblSumTotal->Text = L"รวมทั้งหมด: 0 ทะลาย";
			// 
			// HistoryForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(48)));
			this->ClientSize = System::Drawing::Size(1264, 681);
			this->Controls->Add(this->lblSumTotal);
			this->Controls->Add(this->lblSumRotten);
			this->Controls->Add(this->lblSumEmpty);
			this->Controls->Add(this->lblSumUnripe);
			this->Controls->Add(this->lblSumOverripe);
			this->Controls->Add(this->lblSumUnderripe);
			this->Controls->Add(this->lblSumRipe);
			this->Controls->Add(this->btnResetFilter);
			this->Controls->Add(this->btnFilter);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->dtpEndDate);
			this->Controls->Add(this->dtpStartDate);
			this->Controls->Add(this->btnDeleteAll);
			this->Controls->Add(this->btnDeleteSelected);
			this->Controls->Add(this->btnSaveChanges);
			this->Controls->Add(this->chartHis);
			this->Controls->Add(this->picHisSlip);
			this->Controls->Add(this->picHisImage);
			this->Controls->Add(this->gridHistory);
			this->Name = L"HistoryForm";
			this->Text = L"HistoryForm";
			this->Load += gcnew System::EventHandler(this, &HistoryForm::HistoryForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gridHistory))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picHisImage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picHisSlip))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartHis))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: void CalculateSums() {
	int sEmpty = 0, sOverripe = 0, sRipe = 0, sRotten = 0, sUnderripe = 0, sUnripe = 0;

	// วนลูปอ่านข้อมูลทุกแถวในตาราง
	for (int i = 0; i < gridHistory->Rows->Count; i++) {
		System::Windows::Forms::DataGridViewRow^ row = gridHistory->Rows[i];

		// ข้ามบรรทัดว่างล่างสุด หรือ บรรทัดที่ถูกซ่อนไว้ (จากการกรองวันที่)
		if (row->IsNewRow || !row->Visible) continue;

		// อ่านค่าจากคอลัมน์ต่างๆ ให้ตรงกับที่ตั้งไว้ตอน Load (คอลัมน์ 2 ถึง 7)
		sEmpty += Convert::ToInt32(row->Cells[2]->Value != nullptr && row->Cells[2]->Value->ToString() != "" ? row->Cells[2]->Value->ToString() : L"0");
		sOverripe += Convert::ToInt32(row->Cells[3]->Value != nullptr && row->Cells[3]->Value->ToString() != "" ? row->Cells[3]->Value->ToString() : L"0");
		sRipe += Convert::ToInt32(row->Cells[4]->Value != nullptr && row->Cells[4]->Value->ToString() != "" ? row->Cells[4]->Value->ToString() : L"0");
		sRotten += Convert::ToInt32(row->Cells[5]->Value != nullptr && row->Cells[5]->Value->ToString() != "" ? row->Cells[5]->Value->ToString() : L"0");
		sUnderripe += Convert::ToInt32(row->Cells[6]->Value != nullptr && row->Cells[6]->Value->ToString() != "" ? row->Cells[6]->Value->ToString() : L"0");
		sUnripe += Convert::ToInt32(row->Cells[7]->Value != nullptr && row->Cells[7]->Value->ToString() != "" ? row->Cells[7]->Value->ToString() : L"0");
	}

	int sTotal = sEmpty + sOverripe + sRipe + sRotten + sUnderripe + sUnripe;

	// อัปเดตข้อความลง Label สรุปผลด้านล่าง (ตามชื่อที่คุณเพิ่งตั้งไป)
	lblSumEmpty->Text = L"ทะลายเปล่า: " + sEmpty.ToString();
	lblSumOverripe->Text = L"สุกเกิน: " + sOverripe.ToString();
	lblSumRipe->Text = L"สุกพอดี: " + sRipe.ToString();
	lblSumRotten->Text = L"เน่า: " + sRotten.ToString();
	lblSumUnderripe->Text = L"ห่าม (กึ่งสุก): " + sUnderripe.ToString();
	lblSumUnripe->Text = L"ดิบ: " + sUnripe.ToString();
	lblSumTotal->Text = L"รวมทั้งหมด: " + sTotal.ToString() + L" ทะลาย";
}

	private: System::Void chartHistory_Click(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void dataGridView1_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
}
private: System::Void HistoryForm_Load(System::Object^ sender, System::EventArgs^ e) {
	chartHis->ChartAreas[0]->BackColor = System::Drawing::Color::FromArgb(45, 45, 48);
	chartHis->Series[0]->Points->Clear();

	// 1. สร้างตาราง 6 คลาส + คอลัมน์ไฟล์ TxtPath
	gridHistory->Columns->Clear();
	System::Windows::Forms::DataGridViewCheckBoxColumn^ chkCol = gcnew System::Windows::Forms::DataGridViewCheckBoxColumn();
	chkCol->HeaderText = L"เลือก"; chkCol->Width = 50; gridHistory->Columns->Add(chkCol);
	gridHistory->Columns->Add("Date", L"วันที่");
	gridHistory->Columns->Add("Empty", L"ทะลายเปล่า");
	gridHistory->Columns->Add("Overripe", L"สุกเกิน");
	gridHistory->Columns->Add("Ripe", L"สุกพอดี");
	gridHistory->Columns->Add("Rotten", L"เน่า");
	gridHistory->Columns->Add("Underripe", L"ห่าม");
	gridHistory->Columns->Add("Unripe", L"ดิบ");
	gridHistory->Columns->Add("ImgPath", L"ImgPath");
	gridHistory->Columns->Add("SlipPath", L"SlipPath");
	gridHistory->Columns->Add("TxtPath", L"TxtPath"); // <-- เพิ่มคอลัมน์ซ่อน สำหรับไฟล์ Text

	gridHistory->Columns[1]->Width = 120;
	for (int i = 2; i <= 7; i++) gridHistory->Columns[i]->Width = 65;
	gridHistory->Columns[8]->Visible = false;  // ซ่อนที่อยู่ไฟล์รูป
	gridHistory->Columns[9]->Visible = false;  // ซ่อนที่อยู่สลิป
	gridHistory->Columns[10]->Visible = false; // ซ่อนที่อยู่ไฟล์ Text พิกัด

	// 2. ดึงข้อมูล CSV
	System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
	System::String^ csvPath = docPath + L"\\FFB_History\\HistoryData.csv";

	if (System::IO::File::Exists(csvPath)) {
		array<System::String^>^ lines = System::IO::File::ReadAllLines(csvPath, System::Text::Encoding::UTF8);
		for (int i = 1; i < lines->Length; i++) {
			array<System::String^>^ rowData = lines[i]->Split(',');

			if (rowData->Length >= 10) {
				// ยุคใหม่ล่าสุด (6 คลาส + มีไฟล์ Text พิกัดกรอบ)
				array<System::Object^>^ newRow = gcnew array<System::Object^>(11);
				newRow[0] = false;
				for (int j = 0; j < 10; j++) newRow[j + 1] = rowData[j];
				gridHistory->Rows->Add(newRow);
			}
			else if (rowData->Length == 9) {
				// รองรับประวัติยุคกลาง (6 คลาส แต่ยังไม่มีไฟล์ Text)
				array<System::Object^>^ newRow = gcnew array<System::Object^>(11);
				newRow[0] = false;
				for (int j = 0; j < 9; j++) newRow[j + 1] = rowData[j];
				newRow[10] = L""; // ปล่อยช่อง TxtPath ให้ว่างไว้
				gridHistory->Rows->Add(newRow);
			}
			else if (rowData->Length == 5) {
				// รองรับประวัติยุคเก่าสุด (มีแค่ สุก/ดิบ)
				array<System::Object^>^ newRow = gcnew array<System::Object^>(11);
				newRow[0] = false;
				newRow[1] = rowData[0]; newRow[2] = L"0"; newRow[3] = L"0";
				newRow[4] = rowData[1]; newRow[5] = L"0"; newRow[6] = L"0"; newRow[7] = rowData[2];
				newRow[8] = rowData[3]; newRow[9] = rowData[4]; newRow[10] = L"";
				gridHistory->Rows->Add(newRow);
			}
		}
	}
	CalculateSums();
}


private: System::Void gridHistory_CellClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
	if (e->RowIndex >= 0) {
		System::Windows::Forms::DataGridViewRow^ row = gridHistory->Rows[e->RowIndex];
		if (row->IsNewRow) return;

		int cEmpty = Convert::ToInt32(row->Cells[2]->Value != nullptr && row->Cells[2]->Value->ToString() != "" ? row->Cells[2]->Value->ToString() : L"0");
		int cOverripe = Convert::ToInt32(row->Cells[3]->Value != nullptr && row->Cells[3]->Value->ToString() != "" ? row->Cells[3]->Value->ToString() : L"0");
		int cRipe = Convert::ToInt32(row->Cells[4]->Value != nullptr && row->Cells[4]->Value->ToString() != "" ? row->Cells[4]->Value->ToString() : L"0");
		int cRotten = Convert::ToInt32(row->Cells[5]->Value != nullptr && row->Cells[5]->Value->ToString() != "" ? row->Cells[5]->Value->ToString() : L"0");
		int cUnderripe = Convert::ToInt32(row->Cells[6]->Value != nullptr && row->Cells[6]->Value->ToString() != "" ? row->Cells[6]->Value->ToString() : L"0");
		int cUnripe = Convert::ToInt32(row->Cells[7]->Value != nullptr && row->Cells[7]->Value->ToString() != "" ? row->Cells[7]->Value->ToString() : L"0");

		System::String^ imgPath = row->Cells[8]->Value != nullptr ? row->Cells[8]->Value->ToString() : L"";
		System::String^ slipPath = row->Cells[9]->Value != nullptr ? row->Cells[9]->Value->ToString() : L"";
		// รองรับตารางเก่าที่ยังไม่มี TxtPath
		System::String^ txtPath = (gridHistory->Columns->Count > 10 && row->Cells[10]->Value != nullptr) ? row->Cells[10]->Value->ToString() : L"";

		// อัปเดตกราฟโดนัท... (ปล่อยโค้ดกราฟโดนัทไว้เหมือนเดิม)
		chartHis->Series[0]->Points->Clear();
		if (cRipe > 0) { chartHis->Series[0]->Points->AddXY(L"สุก", cRipe); chartHis->Series[0]->Points[chartHis->Series[0]->Points->Count - 1]->Color = Color::MediumSeaGreen; }
		if (cUnderripe > 0) { chartHis->Series[0]->Points->AddXY(L"ห่าม", cUnderripe); chartHis->Series[0]->Points[chartHis->Series[0]->Points->Count - 1]->Color = Color::Gold; }
		if (cOverripe > 0) { chartHis->Series[0]->Points->AddXY(L"สุกเกิน", cOverripe); chartHis->Series[0]->Points[chartHis->Series[0]->Points->Count - 1]->Color = Color::Orange; }
		if (cUnripe > 0) { chartHis->Series[0]->Points->AddXY(L"ดิบ", cUnripe); chartHis->Series[0]->Points[chartHis->Series[0]->Points->Count - 1]->Color = Color::MediumPurple; }
		if (cEmpty > 0) { chartHis->Series[0]->Points->AddXY(L"ทะลายเปล่า", cEmpty); chartHis->Series[0]->Points[chartHis->Series[0]->Points->Count - 1]->Color = Color::LightCoral; }
		if (cRotten > 0) { chartHis->Series[0]->Points->AddXY(L"เน่า", cRotten); chartHis->Series[0]->Points[chartHis->Series[0]->Points->Count - 1]->Color = Color::Silver; }

		// --- ระบบวาดรูป Real-time ---
		if (System::IO::File::Exists(imgPath)) {
			System::Drawing::Image^ originalImg = System::Drawing::Image::FromFile(imgPath);
			System::Drawing::Bitmap^ drawImg = gcnew System::Drawing::Bitmap(originalImg);
			delete originalImg; // คืนแรม

			// เช็คว่าผู้ใช้คลิกคอลัมน์ไหน (ถ้าคลิก 0 หรือ 1 คือแสดงทั้งหมด)
			int targetClass = -1; // -1 = แสดงทั้งหมด
			if (e->ColumnIndex == 2) targetClass = 0; // Empty
			else if (e->ColumnIndex == 3) targetClass = 1; // Overripe
			else if (e->ColumnIndex == 4) targetClass = 2; // Ripe
			else if (e->ColumnIndex == 5) targetClass = 3; // Rotten
			else if (e->ColumnIndex == 6) targetClass = 4; // Underripe
			else if (e->ColumnIndex == 7) targetClass = 5; // Unripe

			// ถ้ามีไฟล์ Text พิกัด ให้วาดทับรูปต้นฉบับ
			if (System::IO::File::Exists(txtPath)) {
				System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(drawImg);
				array<System::String^>^ lines = System::IO::File::ReadAllLines(txtPath);

				for (int i = 0; i < lines->Length; i++) {
					array<System::String^>^ boxData = lines[i]->Split(',');
					if (boxData->Length == 6) {
						int cid = Convert::ToInt32(boxData[0]);

						// เช็คเงื่อนไข (ถ้าระบุคลาสมา ให้วาดเฉพาะคลาสนั้น ถ้า -1 ให้วาดหมด)
						if (targetClass != -1 && cid != targetClass) continue;

						int x = Convert::ToInt32(boxData[1]); int y = Convert::ToInt32(boxData[2]);
						int w = Convert::ToInt32(boxData[3]); int h = Convert::ToInt32(boxData[4]);
						System::String^ conf = boxData[5];

						System::Drawing::Color bColor; System::String^ bLabel;
						if (cid == 0) { bColor = Color::LightCoral;  bLabel = L"Empty"; }
						else if (cid == 1) { bColor = Color::Orange;      bLabel = L"Overripe"; }
						else if (cid == 2) { bColor = Color::LimeGreen;   bLabel = L"Ripe"; }
						else if (cid == 3) { bColor = Color::Silver;      bLabel = L"Rotten"; }
						else if (cid == 4) { bColor = Color::Gold;        bLabel = L"Underripe"; }
						else if (cid == 5) { bColor = Color::DeepSkyBlue; bLabel = L"Unripe"; }

						System::Drawing::Pen^ pen = gcnew System::Drawing::Pen(bColor, 3.0f);
						g->DrawRectangle(pen, x, y, w, h);
						System::Drawing::Font^ font = gcnew System::Drawing::Font(L"Tahoma", 12, FontStyle::Bold);
						g->DrawString(bLabel + L" " + conf + L"%", font, gcnew System::Drawing::SolidBrush(bColor), x, y - 20);
					}
				}
				delete g;
			}

			if (picHisImage->Image != nullptr) delete picHisImage->Image;
			picHisImage->Image = drawImg;
		}
		else {
			picHisImage->Image = nullptr;
		}

		if (System::IO::File::Exists(slipPath)) picHisSlip->Image = System::Drawing::Image::FromFile(slipPath); else picHisSlip->Image = nullptr;
	}
}


private: System::Void btnDeleteSelected_Click(System::Object^ sender, System::EventArgs^ e) {
	// ต้องวนลูปถอยหลังเวลาลบแถว เพื่อไม่ให้ลำดับ Index รวน
	for (int i = gridHistory->Rows->Count - 1; i >= 0; i--) {
		if (gridHistory->Rows[i]->IsNewRow) continue;

		bool isChecked = false;
		if (gridHistory->Rows[i]->Cells[0]->Value != nullptr) {
			isChecked = System::Convert::ToBoolean(gridHistory->Rows[i]->Cells[0]->Value);
		}

		if (isChecked) {
			gridHistory->Rows->RemoveAt(i); // ลบทิ้งจากหน้าจอ
		}
	}
	// แอบเรียกใช้ปุ่ม Save เพื่ออัปเดตไฟล์ CSV อัตโนมัติ!
	btnSaveChanges_Click(nullptr, nullptr);
	CalculateSums();
}

private: System::Void btnDeleteAll_Click(System::Object^ sender, System::EventArgs^ e) {
	System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show(L"คุณแน่ใจหรือไม่ที่จะลบประวัติทั้งหมด?\n(ข้อมูลจะไม่สามารถกู้คืนได้)", L"ยืนยันการลบ", System::Windows::Forms::MessageBoxButtons::YesNo, System::Windows::Forms::MessageBoxIcon::Warning);

	if (result == System::Windows::Forms::DialogResult::Yes) {
		gridHistory->Rows->Clear(); // ล้างหน้าจอทั้งหมด
		btnSaveChanges_Click(nullptr, nullptr); // อัปเดตไฟล์

		// ล้างรูปและกราฟด้วย
		picHisImage->Image = nullptr;
		picHisSlip->Image = nullptr;
		chartHis->Series[0]->Points->Clear();
		CalculateSums();
	}
}
private: System::Void btnSaveChanges_Click(System::Object^ sender, System::EventArgs^ e) {
	System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
	System::String^ csvPath = docPath + L"\\FFB_History\\HistoryData.csv";

	// เปิดไฟล์แบบ "เขียนทับ (false)" เพื่อเซฟข้อมูลใหม่ทั้งหมดที่อยู่บนหน้าจอลงไป
	System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(csvPath, false, System::Text::Encoding::UTF8);

	// 1. เขียนหัวตารางให้ครบ 10 คอลัมน์ (รวม TxtPath ไว้ท้ายสุด)
	sw->WriteLine(L"Date,Empty,Overripe,Ripe,Rotten,Underripe,Unripe,ImagePath,SlipPath,TxtPath");

	// 2. วนลูปอ่านข้อมูลทุกบรรทัดในตาราง
	for (int i = 0; i < gridHistory->Rows->Count; i++) {
		if (gridHistory->Rows[i]->IsNewRow) continue; // ข้ามบรรทัดว่างที่อยู่ล่างสุด

		System::String^ line = "";

		// วนลูปกวาดข้อมูลตั้งแต่คอลัมน์ 1 (วันที่) จนถึง 10 (TxtPath)
		for (int j = 1; j <= 10; j++) {
			System::String^ val = L"";

			// ดักจับความปลอดภัย: เช็คว่าคอลัมน์มีอยู่จริงไหม (เผื่อเป็นประวัติเก่าๆ) และต้องไม่เป็นค่าว่าง
			if (j < gridHistory->Columns->Count && gridHistory->Rows[i]->Cells[j]->Value != nullptr) {
				val = gridHistory->Rows[i]->Cells[j]->Value->ToString();
			}
			else if (j >= 2 && j <= 7) {
				// ถ้าเป็นคอลัมน์ตัวเลขคลาสปาล์ม (2 ถึง 7) แล้วหาค่าไม่เจอ ให้บังคับใส่ "0" เพื่อไม่ให้พังเวลาคำนวณ
				val = L"0";
			}

			line += val;
			if (j < 10) line += L","; // เติมลูกน้ำ (,) คั่นข้อมูล ยกเว้นคอลัมน์สุดท้าย
		}
		sw->WriteLine(line);
	}
	sw->Close();

	System::Windows::Forms::MessageBox::Show(L"อัปเดตข้อมูลประวัติลงไฟล์สำเร็จ!", L"การบันทึก", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void btnResetFilter_Click(System::Object^ sender, System::EventArgs^ e) {
	gridHistory->CurrentCell = nullptr;

	// โชว์ทุกแถวกลับมาเหมือนเดิม
	for (int i = 0; i < gridHistory->Rows->Count; i++) {
		if (!gridHistory->Rows[i]->IsNewRow) {
			gridHistory->Rows[i]->Visible = true;
		}
	}

	// คำนวณผลรวมของทุกแถว
	CalculateSums();
}
private: System::Void lblSumUnderripe_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void btnFilter_Click(System::Object^ sender, System::EventArgs^ e) {// 1. ดึงวันที่จากปฏิทิน (เริ่ม 00:00:00 ถึง 23:59:59 ของวันสิ้นสุด)
	DateTime startDate = dtpStartDate->Value.Date;
	DateTime endDate = dtpEndDate->Value.Date.AddDays(1).AddSeconds(-1);

	gridHistory->CurrentCell = nullptr; // ป้องกัน Error การซ่อนแถวที่กำลังเลือกอยู่

	// 2. วนลูปเช็ควันที่ทีละแถว
	for (int i = 0; i < gridHistory->Rows->Count; i++) {
		System::Windows::Forms::DataGridViewRow^ row = gridHistory->Rows[i];
		if (row->IsNewRow) continue;

		System::String^ dateStr = row->Cells[1]->Value != nullptr ? row->Cells[1]->Value->ToString() : L"";
		try {
			// แปลงข้อความกลับเป็นวันเวลาจริงๆ
			DateTime rowDate = DateTime::ParseExact(dateStr, L"dd/MM/yyyy HH:mm", System::Globalization::CultureInfo::InvariantCulture);

			// ถ้าเวลาอยู่ในช่วงที่เลือก ให้แสดง ถ้าไม่อยู่ ให้ซ่อน
			if (rowDate >= startDate && rowDate <= endDate) {
				row->Visible = true;
			}
			else {
				row->Visible = false;
			}
		}
		catch (...) {
			row->Visible = false; // ถ้าแปลงวันที่ไม่ผ่าน ให้ซ่อนไว้ก่อน
		}
	}

	// 3. คำนวณผลรวมใหม่เฉพาะแถวที่แสดงอยู่
	CalculateSums();
}
};
}
