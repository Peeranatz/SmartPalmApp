#pragma once
#include "HistoryForm.h"
#include "YoloEngine.h"
#include "SettingsForm.h" // ดึงหน้าต่างตั้งค่ามาใช้งาน
#include <msclr/marshal_cppstd.h>


#pragma managed

namespace plamoil {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Net::Sockets;
	using namespace System::Threading;
	using namespace System::IO;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		delegate void UpdateVideoFrameDelegate(System::Drawing::Image^ frame, System::Drawing::Image^ rawFrame, System::String^ data, System::String^ jsonBoxData);
		Thread^ videoThread;
		bool isVideoRunning = false;
		TcpListener^ videoListener;

		// AI Prediction State
		System::String^ latestAiData = L"";
		System::Diagnostics::Process^ pythonProcess;

		// Preview and File Path Storage
		System::String^ currentPreviewImgPath = L"";
		System::String^ currentPreviewTxtPath = L"";
		System::String^ currentPrintSlipPath = L"";
		System::String^ currentOriginalImgPath = L"";
		System::String^ currentBoxData = L"";
		System::Drawing::Image^ currentRawVideoFrame = nullptr;

		// Default Pricing Constants
		double currentBasePrice = 5.80;
		double dedEmpty = 5.80;
		double dedOverripe = 0.50;
		double dedRipe = 0.00;
		double dedRotten = 5.80;
		double dedUnderripe = 0.20;
		// Dynamic Subtotals
		int curEmpty = 0, curOverripe = 0, curRipe = 0, curRotten = 0, curUnderripe = 0, curUnripe = 0;
		double curNetWeight = 0;
		double curTotalDeduction = 0;
		double curNetRate = 0;
		double curTotalPrice = 0;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	public:
	private: System::Windows::Forms::Label^ lblNetRateShow;
	private: System::Windows::Forms::Label^ lblQualityDeductShow;
	private: System::Windows::Forms::Label^ lblBasePriceShow;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ btnConfirmPurchaseNew;
	private: System::Windows::Forms::Label^ lblTotalPrice;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label9;


	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ txtWeightIn;
	private: System::Windows::Forms::TextBox^ txtWeightOut;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Button^ btnClearNew;
	private: System::Windows::Forms::Label^ lblUploadTextNew;
	private: System::Windows::Forms::PictureBox^ picUploadAreaNew;
	private: System::Windows::Forms::Button^ btnSettings;
	private: System::Windows::Forms::Button^ btnAnalyzeNew;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chartRipeness;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Panel^ pnlDashboard;
	private: System::Windows::Forms::GroupBox^ groupBox6;
	private: System::Windows::Forms::GroupBox^ groupBox5;
	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::Label^ lblDashAvgRate;
	private: System::Windows::Forms::Label^ lblDashTotalPayout;
	private: System::Windows::Forms::Label^ lblDashTotalWeight;
	private: System::Windows::Forms::GroupBox^ groupBox8;
	private: System::Windows::Forms::GroupBox^ groupBox7;
	private: System::Windows::Forms::DataGridView^ dgvHistory;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column5;
	private: System::Windows::Forms::DataGridViewButtonColumn^ Column6;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chartDashQuality;
	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::Label^ lblShowBasePriceTop;
	private: System::Windows::Forms::Button^ btnResetFilter;
	private: System::Windows::Forms::Button^ btnFilterDate;
	private: System::Windows::Forms::DateTimePicker^ dtpEndDate;

	private: System::Windows::Forms::DateTimePicker^ dtpStartDate;
	private: System::Windows::Forms::Button^ btnDeleteAll;
	private: System::Windows::Forms::Button^ btnDeleteRow;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ btnViewAI;
	private: System::Windows::Forms::Panel^ pnlAIPreview;
	private: System::Windows::Forms::Button^ btnCloseAI;


	private: System::Windows::Forms::ComboBox^ cmbAIFilter;
	private: System::Windows::Forms::PictureBox^ picAI;
	private: System::Windows::Forms::Button^ btnDownloadSlip;
	private: System::Windows::Forms::PictureBox^ picSlipPreview;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chartAIPreview;
	private: System::Windows::Forms::Panel^ pnlVideoControl;
	private: System::Windows::Forms::Button^ btnStartVideo;
	private: System::Windows::Forms::ComboBox^ cmbCameraSource;
	private: System::Windows::Forms::Button^ btnStopVideo;
private: System::Windows::Forms::CheckBox^ chkShowMask;
private: System::Windows::Forms::CheckBox^ chkShowBox;
private: System::Windows::Forms::CheckBox^ chkHistMask;
private: System::Windows::Forms::CheckBox^ chkHistBox;



	   // ฟังก์ชันใหม่: บังคับให้รอรับข้อมูลจนครบตามจำนวน Byte ห้ามรับมาแหว่งๆ เด็ดขาด
	private: array<System::Byte>^ ReadExactBytes(NetworkStream^ stream, int count) {
		array<System::Byte>^ buffer = gcnew array<System::Byte>(count);
		int offset = 0;
		while (offset < count) {
			int read = stream->Read(buffer, offset, count - offset);
			if (read == 0) throw gcnew System::Exception(L"Python ตัดการเชื่อมต่อ");
			offset += read;
		}
		return buffer;
	}


	public:
		double dedUnripe = 2.00;

		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			// Initialize Base Price Label
			lblShowBasePriceTop->Text = currentBasePrice.ToString(L"F2") + L" THB";

			// Initialize default Chart properties
			chartRipeness->Series->Clear();
			chartRipeness->Series->Add("Ripeness");
			chartRipeness->Series["Ripeness"]->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Doughnut;
			chartRipeness->ChartAreas[0]->BackColor = System::Drawing::Color::FromArgb(45, 45, 48);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->lblNetRateShow = (gcnew System::Windows::Forms::Label());
			this->lblQualityDeductShow = (gcnew System::Windows::Forms::Label());
			this->lblBasePriceShow = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->btnConfirmPurchaseNew = (gcnew System::Windows::Forms::Button());
			this->lblTotalPrice = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->txtWeightIn = (gcnew System::Windows::Forms::TextBox());
			this->txtWeightOut = (gcnew System::Windows::Forms::TextBox());
			this->btnAnalyzeNew = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->chkShowMask = (gcnew System::Windows::Forms::CheckBox());
			this->chkShowBox = (gcnew System::Windows::Forms::CheckBox());
			this->btnStopVideo = (gcnew System::Windows::Forms::Button());
			this->btnStartVideo = (gcnew System::Windows::Forms::Button());
			this->pnlVideoControl = (gcnew System::Windows::Forms::Panel());
			this->cmbCameraSource = (gcnew System::Windows::Forms::ComboBox());
			this->lblShowBasePriceTop = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->btnClearNew = (gcnew System::Windows::Forms::Button());
			this->lblUploadTextNew = (gcnew System::Windows::Forms::Label());
			this->picUploadAreaNew = (gcnew System::Windows::Forms::PictureBox());
			this->btnSettings = (gcnew System::Windows::Forms::Button());
			this->chartRipeness = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->pnlDashboard = (gcnew System::Windows::Forms::Panel());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnResetFilter = (gcnew System::Windows::Forms::Button());
			this->btnFilterDate = (gcnew System::Windows::Forms::Button());
			this->dtpEndDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->dtpStartDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->chartDashQuality = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->btnViewAI = (gcnew System::Windows::Forms::Button());
			this->btnDeleteAll = (gcnew System::Windows::Forms::Button());
			this->btnDeleteRow = (gcnew System::Windows::Forms::Button());
			this->dgvHistory = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column6 = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->lblDashAvgRate = (gcnew System::Windows::Forms::Label());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->lblDashTotalPayout = (gcnew System::Windows::Forms::Label());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->lblDashTotalWeight = (gcnew System::Windows::Forms::Label());
			this->pnlAIPreview = (gcnew System::Windows::Forms::Panel());
			this->chkHistMask = (gcnew System::Windows::Forms::CheckBox());
			this->chkHistBox = (gcnew System::Windows::Forms::CheckBox());
			this->btnDownloadSlip = (gcnew System::Windows::Forms::Button());
			this->picSlipPreview = (gcnew System::Windows::Forms::PictureBox());
			this->chartAIPreview = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->btnCloseAI = (gcnew System::Windows::Forms::Button());
			this->cmbAIFilter = (gcnew System::Windows::Forms::ComboBox());
			this->picAI = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox3->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->pnlVideoControl->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picUploadAreaNew))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartRipeness))->BeginInit();
			this->pnlDashboard->SuspendLayout();
			this->groupBox8->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartDashQuality))->BeginInit();
			this->groupBox7->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvHistory))->BeginInit();
			this->groupBox6->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->pnlAIPreview->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picSlipPreview))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartAIPreview))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picAI))->BeginInit();
			this->SuspendLayout();
			// 
			// groupBox3
			// 
			this->groupBox3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->groupBox3->Controls->Add(this->lblNetRateShow);
			this->groupBox3->Controls->Add(this->lblQualityDeductShow);
			this->groupBox3->Controls->Add(this->lblBasePriceShow);
			this->groupBox3->Controls->Add(this->label6);
			this->groupBox3->Controls->Add(this->label5);
			this->groupBox3->Controls->Add(this->label4);
			this->groupBox3->Controls->Add(this->label3);
			this->groupBox3->Controls->Add(this->btnConfirmPurchaseNew);
			this->groupBox3->Controls->Add(this->lblTotalPrice);
			this->groupBox3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox3->Location = System::Drawing::Point(855, 457);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(672, 309);
			this->groupBox3->TabIndex = 5;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Financial Calculation";
			// 
			// lblNetRateShow
			// 
			this->lblNetRateShow->AutoSize = true;
			this->lblNetRateShow->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->lblNetRateShow->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblNetRateShow->ForeColor = System::Drawing::Color::SteelBlue;
			this->lblNetRateShow->Location = System::Drawing::Point(604, 106);
			this->lblNetRateShow->Name = L"lblNetRateShow";
			this->lblNetRateShow->Size = System::Drawing::Size(32, 16);
			this->lblNetRateShow->TabIndex = 19;
			this->lblNetRateShow->Text = L"0.00";
			// 
			// lblQualityDeductShow
			// 
			this->lblQualityDeductShow->AutoSize = true;
			this->lblQualityDeductShow->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->lblQualityDeductShow->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblQualityDeductShow->ForeColor = System::Drawing::Color::Red;
			this->lblQualityDeductShow->Location = System::Drawing::Point(604, 62);
			this->lblQualityDeductShow->Name = L"lblQualityDeductShow";
			this->lblQualityDeductShow->Size = System::Drawing::Size(32, 16);
			this->lblQualityDeductShow->TabIndex = 18;
			this->lblQualityDeductShow->Text = L"0.00";
			// 
			// lblBasePriceShow
			// 
			this->lblBasePriceShow->AutoSize = true;
			this->lblBasePriceShow->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->lblBasePriceShow->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblBasePriceShow->ForeColor = System::Drawing::Color::White;
			this->lblBasePriceShow->Location = System::Drawing::Point(604, 28);
			this->lblBasePriceShow->Name = L"lblBasePriceShow";
			this->lblBasePriceShow->Size = System::Drawing::Size(32, 16);
			this->lblBasePriceShow->TabIndex = 17;
			this->lblBasePriceShow->Text = L"0.00";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->label6->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(12, 106);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(143, 16);
			this->label6->TabIndex = 16;
			this->label6->Text = L"NET RATE (THB / KG):";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label5->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(12, 62);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(159, 16);
			this->label5->TabIndex = 15;
			this->label5->Text = L"Quality Deduct (THB / KG):";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label4->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(12, 28);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(136, 16);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Base Price (THB / KG):";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label3->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(12, 148);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(219, 16);
			this->label3->TabIndex = 13;
			this->label3->Text = L"TOTAL PLAYABLE AMOUNT (THB):";
			// 
			// btnConfirmPurchaseNew
			// 
			this->btnConfirmPurchaseNew->BackColor = System::Drawing::Color::Green;
			this->btnConfirmPurchaseNew->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnConfirmPurchaseNew->FlatAppearance->BorderColor = System::Drawing::Color::White;
			this->btnConfirmPurchaseNew->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnConfirmPurchaseNew->Font = (gcnew System::Drawing::Font(L"Tahoma", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnConfirmPurchaseNew->ForeColor = System::Drawing::Color::White;
			this->btnConfirmPurchaseNew->Location = System::Drawing::Point(15, 251);
			this->btnConfirmPurchaseNew->Name = L"btnConfirmPurchaseNew";
			this->btnConfirmPurchaseNew->Size = System::Drawing::Size(642, 43);
			this->btnConfirmPurchaseNew->TabIndex = 12;
			this->btnConfirmPurchaseNew->Text = L"Confirm purchase ➔";
			this->btnConfirmPurchaseNew->UseVisualStyleBackColor = false;
			this->btnConfirmPurchaseNew->Click += gcnew System::EventHandler(this, &MainForm::btnConfirmPurchaseNew_Click);
			// 
			// lblTotalPrice
			// 
			this->lblTotalPrice->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(5)), static_cast<System::Int32>(static_cast<System::Byte>(5)),
				static_cast<System::Int32>(static_cast<System::Byte>(5)));
			this->lblTotalPrice->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblTotalPrice->Font = (gcnew System::Drawing::Font(L"Consolas", 21.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblTotalPrice->ForeColor = System::Drawing::Color::Cyan;
			this->lblTotalPrice->Location = System::Drawing::Point(15, 175);
			this->lblTotalPrice->Name = L"lblTotalPrice";
			this->lblTotalPrice->Size = System::Drawing::Size(642, 62);
			this->lblTotalPrice->TabIndex = 11;
			this->lblTotalPrice->Text = L"0.00";
			this->lblTotalPrice->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->groupBox2->Controls->Add(this->label11);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->txtWeightIn);
			this->groupBox2->Controls->Add(this->txtWeightOut);
			this->groupBox2->Controls->Add(this->btnAnalyzeNew);
			this->groupBox2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox2->Location = System::Drawing::Point(855, 83);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(672, 368);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Scale Information";
			// 
			// label11
			// 
			this->label11->BackColor = System::Drawing::Color::DimGray;
			this->label11->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label11->Font = (gcnew System::Drawing::Font(L"Consolas", 21.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label11->ForeColor = System::Drawing::Color::White;
			this->label11->Location = System::Drawing::Point(34, 243);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(613, 61);
			this->label11->TabIndex = 17;
			this->label11->Text = L"0";
			this->label11->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->label11->Click += gcnew System::EventHandler(this, &MainForm::label11_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->label9->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label9->ForeColor = System::Drawing::Color::White;
			this->label9->Location = System::Drawing::Point(531, 217);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(116, 16);
			this->label9->TabIndex = 22;
			this->label9->Text = L"Net Weight (KG):";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->label1->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(35, 117);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(153, 16);
			this->label1->TabIndex = 18;
			this->label1->Text = L"Tar Weight (OUT) - KG:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->label7->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(31, 19);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(157, 16);
			this->label7->TabIndex = 17;
			this->label7->Text = L"Gross Weight (IN) - KG:";
			// 
			// txtWeightIn
			// 
			this->txtWeightIn->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(18)), static_cast<System::Int32>(static_cast<System::Byte>(18)),
				static_cast<System::Int32>(static_cast<System::Byte>(18)));
			this->txtWeightIn->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->txtWeightIn->Font = (gcnew System::Drawing::Font(L"Consolas", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txtWeightIn->ForeColor = System::Drawing::Color::Cyan;
			this->txtWeightIn->Location = System::Drawing::Point(34, 38);
			this->txtWeightIn->Multiline = true;
			this->txtWeightIn->Name = L"txtWeightIn";
			this->txtWeightIn->Size = System::Drawing::Size(613, 62);
			this->txtWeightIn->TabIndex = 8;
			this->txtWeightIn->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->txtWeightIn->TextChanged += gcnew System::EventHandler(this, &MainForm::txtWeightIn_TextChanged);
			// 
			// txtWeightOut
			// 
			this->txtWeightOut->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(18)), static_cast<System::Int32>(static_cast<System::Byte>(18)),
				static_cast<System::Int32>(static_cast<System::Byte>(18)));
			this->txtWeightOut->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->txtWeightOut->Font = (gcnew System::Drawing::Font(L"Consolas", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txtWeightOut->ForeColor = System::Drawing::Color::Cyan;
			this->txtWeightOut->Location = System::Drawing::Point(34, 136);
			this->txtWeightOut->Multiline = true;
			this->txtWeightOut->Name = L"txtWeightOut";
			this->txtWeightOut->Size = System::Drawing::Size(613, 62);
			this->txtWeightOut->TabIndex = 10;
			this->txtWeightOut->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->txtWeightOut->TextChanged += gcnew System::EventHandler(this, &MainForm::txtWeightOut_TextChanged);
			// 
			// btnAnalyzeNew
			// 
			this->btnAnalyzeNew->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(45)));
			this->btnAnalyzeNew->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnAnalyzeNew->FlatAppearance->BorderColor = System::Drawing::Color::LightGray;
			this->btnAnalyzeNew->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnAnalyzeNew->Font = (gcnew System::Drawing::Font(L"Tahoma", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnAnalyzeNew->ForeColor = System::Drawing::Color::White;
			this->btnAnalyzeNew->Location = System::Drawing::Point(34, 316);
			this->btnAnalyzeNew->Name = L"btnAnalyzeNew";
			this->btnAnalyzeNew->Size = System::Drawing::Size(613, 36);
			this->btnAnalyzeNew->TabIndex = 3;
			this->btnAnalyzeNew->Text = L"📈 Smart Ai Analysis";
			this->btnAnalyzeNew->UseVisualStyleBackColor = false;
			this->btnAnalyzeNew->Click += gcnew System::EventHandler(this, &MainForm::btnAnalyzeNew_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->groupBox1->Controls->Add(this->chkShowMask);
			this->groupBox1->Controls->Add(this->chkShowBox);
			this->groupBox1->Controls->Add(this->btnStopVideo);
			this->groupBox1->Controls->Add(this->btnStartVideo);
			this->groupBox1->Controls->Add(this->pnlVideoControl);
			this->groupBox1->Controls->Add(this->lblShowBasePriceTop);
			this->groupBox1->Controls->Add(this->label10);
			this->groupBox1->Controls->Add(this->btnClearNew);
			this->groupBox1->Controls->Add(this->lblUploadTextNew);
			this->groupBox1->Controls->Add(this->picUploadAreaNew);
			this->groupBox1->Controls->Add(this->btnSettings);
			this->groupBox1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox1->Location = System::Drawing::Point(12, 83);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(788, 513);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Camera / Image Inspection";
			// 
			// chkShowMask
			// 
			this->chkShowMask->AutoSize = true;
			this->chkShowMask->Checked = true;
			this->chkShowMask->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkShowMask->Location = System::Drawing::Point(194, 480);
			this->chkShowMask->Name = L"chkShowMask";
			this->chkShowMask->Size = System::Drawing::Size(138, 17);
			this->chkShowMask->TabIndex = 24;
			this->chkShowMask->Text = L"🟦 แสดงระบายสี (Mask)";
			this->chkShowMask->UseVisualStyleBackColor = true;
			this->chkShowMask->CheckedChanged += gcnew System::EventHandler(this, &MainForm::chkShowMask_CheckedChanged);
			// 
			// chkShowBox
			// 
			this->chkShowBox->AutoSize = true;
			this->chkShowBox->Checked = true;
			this->chkShowBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkShowBox->Location = System::Drawing::Point(213, 461);
			this->chkShowBox->Name = L"chkShowBox";
			this->chkShowBox->Size = System::Drawing::Size(120, 17);
			this->chkShowBox->TabIndex = 23;
			this->chkShowBox->Text = L"🔲 แสดงกรอบ (Box)";
			this->chkShowBox->UseVisualStyleBackColor = true;
			this->chkShowBox->CheckedChanged += gcnew System::EventHandler(this, &MainForm::chkShowBox_CheckedChanged);
			// 
			// btnStopVideo
			// 
			this->btnStopVideo->BackColor = System::Drawing::Color::Maroon;
			this->btnStopVideo->Enabled = false;
			this->btnStopVideo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnStopVideo->ForeColor = System::Drawing::Color::White;
			this->btnStopVideo->Location = System::Drawing::Point(638, 461);
			this->btnStopVideo->Name = L"btnStopVideo";
			this->btnStopVideo->Size = System::Drawing::Size(127, 39);
			this->btnStopVideo->TabIndex = 22;
			this->btnStopVideo->Text = L"🛑 หยุดทำงาน";
			this->btnStopVideo->UseVisualStyleBackColor = false;
			this->btnStopVideo->Click += gcnew System::EventHandler(this, &MainForm::btnStopVideo_Click);
			// 
			// btnStartVideo
			// 
			this->btnStartVideo->BackColor = System::Drawing::Color::ForestGreen;
			this->btnStartVideo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnStartVideo->ForeColor = System::Drawing::Color::White;
			this->btnStartVideo->Location = System::Drawing::Point(338, 461);
			this->btnStartVideo->Name = L"btnStartVideo";
			this->btnStartVideo->Size = System::Drawing::Size(294, 39);
			this->btnStartVideo->TabIndex = 21;
			this->btnStartVideo->Text = L"📹 เปิดกล้อง / วิดีโอ";
			this->btnStartVideo->UseVisualStyleBackColor = false;
			this->btnStartVideo->Click += gcnew System::EventHandler(this, &MainForm::btnStartVideo_Click);
			// 
			// pnlVideoControl
			// 
			this->pnlVideoControl->Controls->Add(this->cmbCameraSource);
			this->pnlVideoControl->Location = System::Drawing::Point(229, 11);
			this->pnlVideoControl->Name = L"pnlVideoControl";
			this->pnlVideoControl->Size = System::Drawing::Size(128, 40);
			this->pnlVideoControl->TabIndex = 20;
			// 
			// cmbCameraSource
			// 
			this->cmbCameraSource->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbCameraSource->FormattingEnabled = true;
			this->cmbCameraSource->Items->AddRange(gcnew cli::array< System::Object^  >(4) {
				L"0: รูปภาพ (.jpg, .png)", L"1: วิดีโอไฟล์ (.mp4)",
					L"2: กล้อง Webcam เครื่อง", L"3: กล้องมือถือ (IP Camera)"
			});
			this->cmbCameraSource->Location = System::Drawing::Point(3, 3);
			this->cmbCameraSource->Name = L"cmbCameraSource";
			this->cmbCameraSource->Size = System::Drawing::Size(121, 21);
			this->cmbCameraSource->TabIndex = 0;
			this->cmbCameraSource->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::cmbCameraSource_SelectedIndexChanged);
			// 
			// lblShowBasePriceTop
			// 
			this->lblShowBasePriceTop->AutoSize = true;
			this->lblShowBasePriceTop->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->lblShowBasePriceTop->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblShowBasePriceTop->ForeColor = System::Drawing::Color::Cyan;
			this->lblShowBasePriceTop->Location = System::Drawing::Point(157, 35);
			this->lblShowBasePriceTop->Name = L"lblShowBasePriceTop";
			this->lblShowBasePriceTop->Size = System::Drawing::Size(35, 16);
			this->lblShowBasePriceTop->TabIndex = 19;
			this->lblShowBasePriceTop->Text = L"0.00";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->label10->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label10->ForeColor = System::Drawing::Color::White;
			this->label10->Location = System::Drawing::Point(17, 35);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(134, 16);
			this->label10->TabIndex = 18;
			this->label10->Text = L"Base Price (Today):";
			// 
			// btnClearNew
			// 
			this->btnClearNew->BackColor = System::Drawing::Color::DarkRed;
			this->btnClearNew->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnClearNew->FlatAppearance->BorderColor = System::Drawing::Color::LightGray;
			this->btnClearNew->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnClearNew->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnClearNew->ForeColor = System::Drawing::Color::White;
			this->btnClearNew->Location = System::Drawing::Point(20, 461);
			this->btnClearNew->Name = L"btnClearNew";
			this->btnClearNew->Size = System::Drawing::Size(97, 40);
			this->btnClearNew->TabIndex = 5;
			this->btnClearNew->Text = L"Clear";
			this->btnClearNew->UseVisualStyleBackColor = false;
			this->btnClearNew->Click += gcnew System::EventHandler(this, &MainForm::btnClearNew_Click);
			// 
			// lblUploadTextNew
			// 
			this->lblUploadTextNew->AutoSize = true;
			this->lblUploadTextNew->BackColor = System::Drawing::Color::Black;
			this->lblUploadTextNew->Cursor = System::Windows::Forms::Cursors::Hand;
			this->lblUploadTextNew->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblUploadTextNew->ForeColor = System::Drawing::Color::White;
			this->lblUploadTextNew->Location = System::Drawing::Point(225, 243);
			this->lblUploadTextNew->Name = L"lblUploadTextNew";
			this->lblUploadTextNew->Size = System::Drawing::Size(321, 24);
			this->lblUploadTextNew->TabIndex = 4;
			this->lblUploadTextNew->Text = L"กรุณาเลือกรูปแบบการทำงานจากเมนูด้านบน";
			this->lblUploadTextNew->Click += gcnew System::EventHandler(this, &MainForm::lblUploadTextNew_Click);
			// 
			// picUploadAreaNew
			// 
			this->picUploadAreaNew->BackColor = System::Drawing::Color::Black;
			this->picUploadAreaNew->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->picUploadAreaNew->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->picUploadAreaNew->Cursor = System::Windows::Forms::Cursors::Hand;
			this->picUploadAreaNew->Location = System::Drawing::Point(20, 57);
			this->picUploadAreaNew->Name = L"picUploadAreaNew";
			this->picUploadAreaNew->Size = System::Drawing::Size(745, 393);
			this->picUploadAreaNew->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->picUploadAreaNew->TabIndex = 0;
			this->picUploadAreaNew->TabStop = false;
			this->picUploadAreaNew->Click += gcnew System::EventHandler(this, &MainForm::picUploadAreaNew_Click);
			// 
			// btnSettings
			// 
			this->btnSettings->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(45)));
			this->btnSettings->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSettings->FlatAppearance->BorderColor = System::Drawing::Color::LightGray;
			this->btnSettings->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSettings->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnSettings->ForeColor = System::Drawing::Color::White;
			this->btnSettings->Location = System::Drawing::Point(657, 19);
			this->btnSettings->Name = L"btnSettings";
			this->btnSettings->Size = System::Drawing::Size(108, 32);
			this->btnSettings->TabIndex = 3;
			this->btnSettings->Text = L"⚙️ Config";
			this->btnSettings->UseVisualStyleBackColor = false;
			this->btnSettings->Click += gcnew System::EventHandler(this, &MainForm::btnSettings_Click);
			// 
			// chartRipeness
			// 
			this->chartRipeness->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(48)));
			chartArea1->Name = L"ChartArea1";
			this->chartRipeness->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chartRipeness->Legends->Add(legend1);
			this->chartRipeness->Location = System::Drawing::Point(12, 605);
			this->chartRipeness->Name = L"chartRipeness";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Doughnut;
			series1->EmptyPointStyle->LabelForeColor = System::Drawing::Color::White;
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			this->chartRipeness->Series->Add(series1);
			this->chartRipeness->Size = System::Drawing::Size(788, 177);
			this->chartRipeness->TabIndex = 0;
			this->chartRipeness->Text = L"chart1";
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(45)));
			this->button1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button1->FlatAppearance->BorderColor = System::Drawing::Color::LightGray;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->ForeColor = System::Drawing::Color::White;
			this->button1->Location = System::Drawing::Point(12, 41);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(207, 32);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Data Entry (Operation)";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(45)));
			this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button2->FlatAppearance->BorderColor = System::Drawing::Color::LightGray;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->ForeColor = System::Drawing::Color::White;
			this->button2->Location = System::Drawing::Point(225, 41);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(246, 32);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Tranaction Log (Dashboard)";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// label12
			// 
			this->label12->BackColor = System::Drawing::Color::SteelBlue;
			this->label12->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label12->Font = (gcnew System::Drawing::Font(L"Consolas", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label12->ForeColor = System::Drawing::Color::White;
			this->label12->Location = System::Drawing::Point(0, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(1542, 31);
			this->label12->TabIndex = 12;
			this->label12->Text = L"SmartPalm AI ";
			this->label12->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			// 
			// pnlDashboard
			// 
			this->pnlDashboard->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->pnlDashboard->Controls->Add(this->label8);
			this->pnlDashboard->Controls->Add(this->label2);
			this->pnlDashboard->Controls->Add(this->btnResetFilter);
			this->pnlDashboard->Controls->Add(this->btnFilterDate);
			this->pnlDashboard->Controls->Add(this->dtpEndDate);
			this->pnlDashboard->Controls->Add(this->dtpStartDate);
			this->pnlDashboard->Controls->Add(this->label13);
			this->pnlDashboard->Controls->Add(this->groupBox8);
			this->pnlDashboard->Controls->Add(this->groupBox7);
			this->pnlDashboard->Controls->Add(this->groupBox6);
			this->pnlDashboard->Controls->Add(this->groupBox5);
			this->pnlDashboard->Controls->Add(this->groupBox4);
			this->pnlDashboard->Location = System::Drawing::Point(0, 79);
			this->pnlDashboard->Name = L"pnlDashboard";
			this->pnlDashboard->Size = System::Drawing::Size(1542, 703);
			this->pnlDashboard->TabIndex = 13;
			this->pnlDashboard->Visible = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::LightCoral;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(1374, 12);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(19, 16);
			this->label8->TabIndex = 12;
			this->label8->Text = L"ถึง";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Green;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(1210, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(33, 16);
			this->label2->TabIndex = 11;
			this->label2->Text = L"ตั้งแต่";
			// 
			// btnResetFilter
			// 
			this->btnResetFilter->Location = System::Drawing::Point(1117, 32);
			this->btnResetFilter->Name = L"btnResetFilter";
			this->btnResetFilter->Size = System::Drawing::Size(75, 23);
			this->btnResetFilter->TabIndex = 10;
			this->btnResetFilter->Text = L"Reset";
			this->btnResetFilter->UseVisualStyleBackColor = true;
			this->btnResetFilter->Click += gcnew System::EventHandler(this, &MainForm::btnResetFilter_Click);
			// 
			// btnFilterDate
			// 
			this->btnFilterDate->Location = System::Drawing::Point(1117, 6);
			this->btnFilterDate->Name = L"btnFilterDate";
			this->btnFilterDate->Size = System::Drawing::Size(75, 23);
			this->btnFilterDate->TabIndex = 9;
			this->btnFilterDate->Text = L"🔍 Search";
			this->btnFilterDate->UseVisualStyleBackColor = true;
			this->btnFilterDate->Click += gcnew System::EventHandler(this, &MainForm::btnFilterDate_Click);
			// 
			// dtpEndDate
			// 
			this->dtpEndDate->CustomFormat = L"dd/MM/yyyy";
			this->dtpEndDate->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dtpEndDate->Location = System::Drawing::Point(1405, 9);
			this->dtpEndDate->Name = L"dtpEndDate";
			this->dtpEndDate->Size = System::Drawing::Size(95, 20);
			this->dtpEndDate->TabIndex = 8;
			// 
			// dtpStartDate
			// 
			this->dtpStartDate->CustomFormat = L"dd/MM/yyyy";
			this->dtpStartDate->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dtpStartDate->Location = System::Drawing::Point(1259, 9);
			this->dtpStartDate->Name = L"dtpStartDate";
			this->dtpStartDate->Size = System::Drawing::Size(98, 20);
			this->dtpStartDate->TabIndex = 7;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label13->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->label13->Location = System::Drawing::Point(20, 19);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(171, 20);
			this->label13->TabIndex = 6;
			this->label13->Text = L"Daily Summary Data";
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->chartDashQuality);
			this->groupBox8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox8->Location = System::Drawing::Point(742, 191);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(750, 477);
			this->groupBox8->TabIndex = 5;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"Aggregate Quality";
			// 
			// chartDashQuality
			// 
			chartArea2->Name = L"ChartArea1";
			this->chartDashQuality->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->chartDashQuality->Legends->Add(legend2);
			this->chartDashQuality->Location = System::Drawing::Point(22, 28);
			this->chartDashQuality->Name = L"chartDashQuality";
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Doughnut;
			series2->Legend = L"Legend1";
			series2->Name = L"Series1";
			this->chartDashQuality->Series->Add(series2);
			this->chartDashQuality->Size = System::Drawing::Size(702, 428);
			this->chartDashQuality->TabIndex = 0;
			this->chartDashQuality->Text = L"chart1";
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->btnViewAI);
			this->groupBox7->Controls->Add(this->btnDeleteAll);
			this->groupBox7->Controls->Add(this->btnDeleteRow);
			this->groupBox7->Controls->Add(this->dgvHistory);
			this->groupBox7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox7->Location = System::Drawing::Point(20, 191);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(688, 477);
			this->groupBox7->TabIndex = 4;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"DataGrid - Transaction Log";
			// 
			// btnViewAI
			// 
			this->btnViewAI->Location = System::Drawing::Point(366, 13);
			this->btnViewAI->Name = L"btnViewAI";
			this->btnViewAI->Size = System::Drawing::Size(133, 23);
			this->btnViewAI->TabIndex = 3;
			this->btnViewAI->Text = L"👁️ ดูผลสแกน AI เชิงลึก";
			this->btnViewAI->UseVisualStyleBackColor = true;
			this->btnViewAI->Click += gcnew System::EventHandler(this, &MainForm::btnViewAI_Click);
			// 
			// btnDeleteAll
			// 
			this->btnDeleteAll->BackColor = System::Drawing::Color::Maroon;
			this->btnDeleteAll->ForeColor = System::Drawing::Color::White;
			this->btnDeleteAll->Location = System::Drawing::Point(589, 13);
			this->btnDeleteAll->Name = L"btnDeleteAll";
			this->btnDeleteAll->Size = System::Drawing::Size(75, 23);
			this->btnDeleteAll->TabIndex = 2;
			this->btnDeleteAll->Text = L"⚠️ DeleteAll";
			this->btnDeleteAll->UseVisualStyleBackColor = false;
			this->btnDeleteAll->Click += gcnew System::EventHandler(this, &MainForm::btnDeleteAll_Click);
			// 
			// btnDeleteRow
			// 
			this->btnDeleteRow->BackColor = System::Drawing::Color::LightCoral;
			this->btnDeleteRow->ForeColor = System::Drawing::Color::White;
			this->btnDeleteRow->Location = System::Drawing::Point(507, 13);
			this->btnDeleteRow->Name = L"btnDeleteRow";
			this->btnDeleteRow->Size = System::Drawing::Size(68, 23);
			this->btnDeleteRow->TabIndex = 1;
			this->btnDeleteRow->Text = L"🗑️ Delete";
			this->btnDeleteRow->UseVisualStyleBackColor = false;
			this->btnDeleteRow->Click += gcnew System::EventHandler(this, &MainForm::btnDeleteRow_Click);
			// 
			// dgvHistory
			// 
			this->dgvHistory->BackgroundColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(18)), static_cast<System::Int32>(static_cast<System::Byte>(18)),
				static_cast<System::Int32>(static_cast<System::Byte>(18)));
			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(45)));
			dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle1->ForeColor = System::Drawing::Color::White;
			dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dgvHistory->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
			this->dgvHistory->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dgvHistory->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(6) {
				this->Column1, this->Column2,
					this->Column3, this->Column4, this->Column5, this->Column6
			});
			this->dgvHistory->EnableHeadersVisualStyles = false;
			this->dgvHistory->Location = System::Drawing::Point(20, 42);
			this->dgvHistory->Name = L"dgvHistory";
			this->dgvHistory->Size = System::Drawing::Size(644, 414);
			this->dgvHistory->TabIndex = 0;
			this->dgvHistory->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::dgvHistory_CellClick);
			this->dgvHistory->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::dgvHistory_CellContentClick);
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"TX_ID";
			this->Column1->Name = L"Column1";
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"TIME";
			this->Column2->Name = L"Column2";
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"NET_WT";
			this->Column3->Name = L"Column3";
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"RATE";
			this->Column4->Name = L"Column4";
			// 
			// Column5
			// 
			this->Column5->HeaderText = L"TOTAL(THB)";
			this->Column5->Name = L"Column5";
			// 
			// Column6
			// 
			this->Column6->HeaderText = L"ACT";
			this->Column6->Name = L"Column6";
			this->Column6->Text = L"Print";
			this->Column6->UseColumnTextForButtonValue = true;
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->lblDashAvgRate);
			this->groupBox6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox6->Location = System::Drawing::Point(1037, 67);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(455, 90);
			this->groupBox6->TabIndex = 3;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Average Rate(THB/KG)";
			// 
			// lblDashAvgRate
			// 
			this->lblDashAvgRate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblDashAvgRate->ForeColor = System::Drawing::Color::LightCoral;
			this->lblDashAvgRate->Location = System::Drawing::Point(6, 16);
			this->lblDashAvgRate->Name = L"lblDashAvgRate";
			this->lblDashAvgRate->Size = System::Drawing::Size(443, 59);
			this->lblDashAvgRate->TabIndex = 2;
			this->lblDashAvgRate->Text = L"0.00";
			this->lblDashAvgRate->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->lblDashTotalPayout);
			this->groupBox5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox5->Location = System::Drawing::Point(527, 67);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(455, 90);
			this->groupBox5->TabIndex = 3;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Total Playout(TH)";
			// 
			// lblDashTotalPayout
			// 
			this->lblDashTotalPayout->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblDashTotalPayout->ForeColor = System::Drawing::Color::LimeGreen;
			this->lblDashTotalPayout->Location = System::Drawing::Point(6, 16);
			this->lblDashTotalPayout->Name = L"lblDashTotalPayout";
			this->lblDashTotalPayout->Size = System::Drawing::Size(443, 59);
			this->lblDashTotalPayout->TabIndex = 1;
			this->lblDashTotalPayout->Text = L"0.00";
			this->lblDashTotalPayout->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->lblDashTotalWeight);
			this->groupBox4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)), static_cast<System::Int32>(static_cast<System::Byte>(178)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->groupBox4->Location = System::Drawing::Point(20, 67);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(455, 90);
			this->groupBox4->TabIndex = 2;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Total Weight IN (KG)";
			// 
			// lblDashTotalWeight
			// 
			this->lblDashTotalWeight->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblDashTotalWeight->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(102)),
				static_cast<System::Int32>(static_cast<System::Byte>(178)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->lblDashTotalWeight->Location = System::Drawing::Point(6, 16);
			this->lblDashTotalWeight->Name = L"lblDashTotalWeight";
			this->lblDashTotalWeight->Size = System::Drawing::Size(443, 59);
			this->lblDashTotalWeight->TabIndex = 0;
			this->lblDashTotalWeight->Text = L"0";
			this->lblDashTotalWeight->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// pnlAIPreview
			// 
			this->pnlAIPreview->Controls->Add(this->chkHistMask);
			this->pnlAIPreview->Controls->Add(this->chkHistBox);
			this->pnlAIPreview->Controls->Add(this->btnDownloadSlip);
			this->pnlAIPreview->Controls->Add(this->picSlipPreview);
			this->pnlAIPreview->Controls->Add(this->chartAIPreview);
			this->pnlAIPreview->Controls->Add(this->btnCloseAI);
			this->pnlAIPreview->Controls->Add(this->cmbAIFilter);
			this->pnlAIPreview->Controls->Add(this->picAI);
			this->pnlAIPreview->Location = System::Drawing::Point(0, 79);
			this->pnlAIPreview->Name = L"pnlAIPreview";
			this->pnlAIPreview->Size = System::Drawing::Size(1542, 700);
			this->pnlAIPreview->TabIndex = 14;
			this->pnlAIPreview->Visible = false;
			// 
			// chkHistMask
			// 
			this->chkHistMask->AutoSize = true;
			this->chkHistMask->Checked = true;
			this->chkHistMask->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkHistMask->Location = System::Drawing::Point(158, 32);
			this->chkHistMask->Name = L"chkHistMask";
			this->chkHistMask->Size = System::Drawing::Size(138, 17);
			this->chkHistMask->TabIndex = 7;
			this->chkHistMask->Text = L"🟦 แสดงระบายสี (Mask)";
			this->chkHistMask->UseVisualStyleBackColor = true;
			this->chkHistMask->CheckedChanged += gcnew System::EventHandler(this, &MainForm::chkHistMask_CheckedChanged);
			// 
			// chkHistBox
			// 
			this->chkHistBox->AutoSize = true;
			this->chkHistBox->Checked = true;
			this->chkHistBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkHistBox->Location = System::Drawing::Point(158, 9);
			this->chkHistBox->Name = L"chkHistBox";
			this->chkHistBox->Size = System::Drawing::Size(120, 17);
			this->chkHistBox->TabIndex = 6;
			this->chkHistBox->Text = L"🔲 แสดงกรอบ (Box)";
			this->chkHistBox->UseVisualStyleBackColor = true;
			this->chkHistBox->CheckedChanged += gcnew System::EventHandler(this, &MainForm::chkHistBox_CheckedChanged);
			// 
			// btnDownloadSlip
			// 
			this->btnDownloadSlip->Location = System::Drawing::Point(1427, 644);
			this->btnDownloadSlip->Name = L"btnDownloadSlip";
			this->btnDownloadSlip->Size = System::Drawing::Size(103, 23);
			this->btnDownloadSlip->TabIndex = 5;
			this->btnDownloadSlip->Text = L"💾 โหลดใบเสร็จ";
			this->btnDownloadSlip->UseVisualStyleBackColor = true;
			this->btnDownloadSlip->Click += gcnew System::EventHandler(this, &MainForm::btnDownloadSlip_Click);
			// 
			// picSlipPreview
			// 
			this->picSlipPreview->Location = System::Drawing::Point(1264, 51);
			this->picSlipPreview->Name = L"picSlipPreview";
			this->picSlipPreview->Size = System::Drawing::Size(266, 587);
			this->picSlipPreview->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->picSlipPreview->TabIndex = 4;
			this->picSlipPreview->TabStop = false;
			// 
			// chartAIPreview
			// 
			chartArea3->Name = L"ChartArea1";
			this->chartAIPreview->ChartAreas->Add(chartArea3);
			legend3->Name = L"Legend1";
			this->chartAIPreview->Legends->Add(legend3);
			this->chartAIPreview->Location = System::Drawing::Point(776, 55);
			this->chartAIPreview->Name = L"chartAIPreview";
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Doughnut;
			series3->Legend = L"Legend1";
			series3->Name = L"Series1";
			this->chartAIPreview->Series->Add(series3);
			this->chartAIPreview->Size = System::Drawing::Size(467, 626);
			this->chartAIPreview->TabIndex = 3;
			this->chartAIPreview->Text = L"chart1";
			this->chartAIPreview->Click += gcnew System::EventHandler(this, &MainForm::chartAIPreview_Click);
			// 
			// btnCloseAI
			// 
			this->btnCloseAI->BackColor = System::Drawing::Color::Maroon;
			this->btnCloseAI->ForeColor = System::Drawing::Color::Transparent;
			this->btnCloseAI->Location = System::Drawing::Point(1411, 10);
			this->btnCloseAI->Name = L"btnCloseAI";
			this->btnCloseAI->Size = System::Drawing::Size(101, 23);
			this->btnCloseAI->TabIndex = 2;
			this->btnCloseAI->Text = L"❌ ปิดหน้าต่าง";
			this->btnCloseAI->UseVisualStyleBackColor = false;
			this->btnCloseAI->Click += gcnew System::EventHandler(this, &MainForm::btnCloseAI_Click);
			// 
			// cmbAIFilter
			// 
			this->cmbAIFilter->FormattingEnabled = true;
			this->cmbAIFilter->Items->AddRange(gcnew cli::array< System::Object^  >(7) {
				L"โชว์ทั้งหมด (All)", L"0: ทะลายเปล่า (Empty)",
					L"1: สุกเกิน (Overripe)", L"2: สุก (Ripe)", L"3: เน่า (Rotten)", L"4: ห่าม (Underripe)", L"5: ดิบ (Unripe)"
			});
			this->cmbAIFilter->Location = System::Drawing::Point(18, 7);
			this->cmbAIFilter->Name = L"cmbAIFilter";
			this->cmbAIFilter->Size = System::Drawing::Size(121, 21);
			this->cmbAIFilter->TabIndex = 1;
			this->cmbAIFilter->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::cmbAIFilter_SelectedIndexChanged);
			// 
			// picAI
			// 
			this->picAI->BackColor = System::Drawing::Color::Black;
			this->picAI->Location = System::Drawing::Point(18, 55);
			this->picAI->Name = L"picAI";
			this->picAI->Size = System::Drawing::Size(740, 626);
			this->picAI->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->picAI->TabIndex = 0;
			this->picAI->TabStop = false;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(45)),
				static_cast<System::Int32>(static_cast<System::Byte>(48)));
			this->ClientSize = System::Drawing::Size(1542, 778);
			this->Controls->Add(this->pnlAIPreview);
			this->Controls->Add(this->pnlDashboard);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->chartRipeness);
			this->Name = L"MainForm";
			this->Text = L"Smart FFB Ripeness Grading System";
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->pnlVideoControl->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picUploadAreaNew))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartRipeness))->EndInit();
			this->pnlDashboard->ResumeLayout(false);
			this->pnlDashboard->PerformLayout();
			this->groupBox8->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartDashQuality))->EndInit();
			this->groupBox7->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvHistory))->EndInit();
			this->groupBox6->ResumeLayout(false);
			this->groupBox5->ResumeLayout(false);
			this->groupBox4->ResumeLayout(false);
			this->pnlAIPreview->ResumeLayout(false);
			this->pnlAIPreview->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picSlipPreview))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartAIPreview))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picAI))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void splitContainer1_Panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}
	private: System::Void picUploadAreaNew_Click(System::Object^ sender, System::EventArgs^ e) {
		// ปิดการคลิกที่จอแล้ว ย้ายไปรวมที่ปุ่ม btnStartVideo ทั้งหมด
	}
	private: System::Void lblRipeCount_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void btnAnalyzeNew_Click(System::Object^ sender, System::EventArgs^ e) {
		if (System::String::IsNullOrEmpty(latestAiData)) {
			MessageBox::Show(L"ยังไม่มีข้อมูลจากกล้องครับ กรุณากดปุ่มเปิดกล้องและสแกนก่อนครับ", L"Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}

		try {
			// Parse incoming socket payload into class buckets
			array<System::String^>^ counts = latestAiData->Split('|');

			if (counts->Length >= 6) {
				// สุก|ห่าม|สุกเกิน|ดิบ|ทะลายเปล่า|เน่า
				int cRipe = Convert::ToInt32(counts[0]);
				int cUnderripe = Convert::ToInt32(counts[1]);
				int cOverripe = Convert::ToInt32(counts[2]);
				int cUnripe = Convert::ToInt32(counts[3]);
				int cEmpty = Convert::ToInt32(counts[4]);
				int cRotten = Convert::ToInt32(counts[5]);

				// Process core pricing and dispatch to transaction logic
				ProcessAndCalculateData(cEmpty, cOverripe, cRipe, cRotten, cUnderripe, cUnripe);

				// Block execution for Video and Camera Sources (Wait for Image payload)
				if (cmbCameraSource->SelectedIndex != 0) {
					if (picUploadAreaNew->Image != nullptr) {
						System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
						System::String^ tempFolder = docPath + L"\\FFB_History\\Images";
						if (!System::IO::Directory::Exists(tempFolder)) System::IO::Directory::CreateDirectory(tempFolder);

						currentOriginalImgPath = tempFolder + L"\\TEMP_VIDEO_" + System::DateTime::Now.ToString(L"yyyyMMdd_HHmmss") + L".jpg";
						
						// Clone source frame for independent History interaction
						if (currentRawVideoFrame != nullptr) {
							currentRawVideoFrame->Save(currentOriginalImgPath, System::Drawing::Imaging::ImageFormat::Jpeg);
						} else if (picUploadAreaNew->Image != nullptr) {
							picUploadAreaNew->Image->Save(currentOriginalImgPath, System::Drawing::Imaging::ImageFormat::Jpeg);
						}
					}
					if (currentBoxData == nullptr) currentBoxData = gcnew System::String(L"");
				}

				MessageBox::Show(L"ดึงข้อมูลวิดีโอลงตารางและคำนวณเงินสำเร็จเรียบร้อย!\n(พร้อมกดบันทึกข้อมูลแล้วครับ)", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
		}
		catch (System::Exception^ ex) {
			MessageBox::Show(L"เกิดข้อผิดพลาด: " + ex->Message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
	private: System::Void splitContainer1_Panel2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}
	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
		// Initialize Default Dashboard Settings
		chartRipeness->Series->Clear();
		chartRipeness->Series->Add("Ripeness");
		chartRipeness->Series["Ripeness"]->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Doughnut;
		chartRipeness->ChartAreas[0]->BackColor = System::Drawing::Color::FromArgb(45, 45, 48);

		pnlDashboard->Visible = false;
		lblShowBasePriceTop->Text = currentBasePrice.ToString(L"F2") + L" THB";
	}

	private: System::Void lblUnripeCount_Click(System::Object^ sender, System::EventArgs^ e) {
	}

	// Safe Deep-Copy wrapper for converting unmanaged OpenCV primitives to managed GDI+ Bitmaps
	private: System::Drawing::Bitmap^ MatToBitmap(cv::Mat mat) {
		if (mat.empty()) return nullptr;

		// Link unmanaged pointer
		System::Drawing::Bitmap^ tempBmp = gcnew System::Drawing::Bitmap(
			mat.cols, mat.rows, mat.step,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb,
			(System::IntPtr)mat.data);

		// Materialize isolated object
		System::Drawing::Bitmap^ finalBmp = gcnew System::Drawing::Bitmap(tempBmp->Width, tempBmp->Height);
		System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(finalBmp);

		g->DrawImage(tempBmp, 0, 0);

		delete g;
		delete tempBmp;

		return finalBmp;
	}
	private: System::Void btnUploadVideo_Click(System::Object^ sender, System::EventArgs^ e) {
		// 1. สร้างหน้าต่างสำหรับเลือกไฟล์
		System::Windows::Forms::OpenFileDialog^ openFileDialog = gcnew System::Windows::Forms::OpenFileDialog();

		// 2. กรองให้เลือกได้เฉพาะไฟล์วิดีโอ
		openFileDialog->Filter = L"Video Files (*.mp4;*.avi;*.mov)|*.mp4;*.avi;*.mov";
		openFileDialog->Title = L"เลือกไฟล์วิดีโอเพื่อจำลองการสแกน";

		// 3. ถ้าผู้ใช้เลือกวิดีโอและกด OK
		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			// ดึงที่อยู่ไฟล์ (Path) มาเก็บไว้
			System::String^ videoPath = openFileDialog->FileName;

			System::Windows::Forms::MessageBox::Show(L"เลือกไฟล์วิดีโอสำเร็จ!\nพาร์ท: " + videoPath + L"\n\n(หมายเหตุ: การเล่นวิดีโอจะทำงานได้หลังจากเราเชื่อมต่อ OpenCV ในขั้นตอนต่อไปครับ)", L"เตรียมพร้อมสแกนวิดีโอ", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
		}
	}
	private: System::Void btnHistory_Click(System::Object^ sender, System::EventArgs^ e) {
		// สร้างตัวแปรเรียกฟอร์มประวัติ และสั่งให้โชว์ขึ้นมา
		HistoryForm^ historyForm = gcnew HistoryForm();
		historyForm->ShowDialog();
	}
	private: System::Void btnSaveSlip_Click_2(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void lblRottenCount_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void btnSettings_Click(System::Object^ sender, System::EventArgs^ e) {
	plamoil::SettingsForm^ setForm = gcnew plamoil::SettingsForm(currentBasePrice, dedEmpty, dedOverripe, dedRipe, dedRotten, dedUnderripe, dedUnripe);

	if (setForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		// Retain saved preferences
		currentBasePrice = setForm->BasePrice;
		dedEmpty = setForm->DedEmpty;
		dedOverripe = setForm->DedOverripe;
		dedRipe = setForm->DedRipe;
		dedRotten = setForm->DedRotten;
		dedUnderripe = setForm->DedUnderripe;
		dedUnripe = setForm->DedUnripe;
		MessageBox::Show(L"บันทึกสูตรราคาเรียบร้อยแล้ว!", L"สำเร็จ", MessageBoxButtons::OK, MessageBoxIcon::Information);
		lblShowBasePriceTop->Text = currentBasePrice.ToString(L"F2") + L" THB";
	}
}
private: System::Void splitContainer1_SplitterMoved(System::Object^ sender, System::Windows::Forms::SplitterEventArgs^ e) {
}
private: System::Void label11_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void lblUploadTextNew_Click(System::Object^ sender, System::EventArgs^ e) {
	picUploadAreaNew_Click(sender, e); // สั่งให้เหมือนเราคลิกที่รูปภาพเลย
}
private: System::Void btnClearNew_Click(System::Object^ sender, System::EventArgs^ e) {
	// 1. Clear active UI rendering elements
	chartRipeness->Series[0]->Points->Clear();

	// 2. Clear picture display
	if (picUploadAreaNew->Image != nullptr) {
		delete picUploadAreaNew->Image;
		picUploadAreaNew->Image = nullptr;
	}
	lblUploadTextNew->Visible = true;
	lblUploadTextNew->Text = L"กรุณาเลือกรูปแบบการทำงานจากเมนูด้านล่าง";

	// 3. Clear metric displays
	lblBasePriceShow->Text = L"0.00";
	lblQualityDeductShow->Text = L"0.00";
	lblNetRateShow->Text = L"0.00";
	lblTotalPrice->Text = L"0.00";

	label11->Text = L"";

	// 4. Reset AI and State Variables
	latestAiData = L"";
	currentOriginalImgPath = L"";
	currentBoxData = nullptr;
	curNetWeight = 0;
	curTotalDeduction = 0;
	curNetRate = 0;
	curTotalPrice = 0;
	curEmpty = 0; curOverripe = 0; curRipe = 0; curRotten = 0; curUnderripe = 0; curUnripe = 0;

	// 5. Restore standard navigation UI
	cmbCameraSource->SelectedIndex = -1;
	cmbCameraSource->Enabled = true;
	btnStartVideo->Text = L"เริ่มทำงาน";
	btnStartVideo->Enabled = true;
	btnStopVideo->Enabled = false;
}
	// Internal helper to calculate active weight adjustments
	private: void UpdateNetWeight() {
		double wIn = 0;
		double wOut = 0;

		// Try-catch block to suppress conversion exceptions if UI text box fields carry malformed elements
		try {
			if (!String::IsNullOrWhiteSpace(txtWeightIn->Text)) wIn = Convert::ToDouble(txtWeightIn->Text);
		}
		catch (...) { wIn = 0; }

		try {
			if (!String::IsNullOrWhiteSpace(txtWeightOut->Text)) wOut = Convert::ToDouble(txtWeightOut->Text);
		}
		catch (...) { wOut = 0; }

		// คำนวณน้ำหนักสุทธิ (รถเข้า - รถออก)
		double net = wIn - wOut;
		if (net < 0) net = 0; // ป้องกันไม่ให้ตัวเลขติดลบ

		// อัปเดตโชว์ที่ label11 พร้อมใส่ลูกน้ำหลักพัน (N0) ให้สวยงาม
		label11->Text = net.ToString(L"N0");
	}
	private: System::Void txtWeightIn_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		UpdateNetWeight();
	}
	private: System::Void txtWeightOut_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		UpdateNetWeight();
	}
	private: System::Void btnConfirmPurchaseNew_Click(System::Object^ sender, System::EventArgs^ e) {
		// Input sanity check before creating records
		if (curTotalPrice <= 0 || currentOriginalImgPath == L"") {
			MessageBox::Show(L"ไม่มีข้อมูลให้บันทึก กรุณาชั่งน้ำหนักและวิเคราะห์ AI ก่อนครับ", L"แจ้งเตือน", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}

		// Initialize Document Output Directory
		System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
		System::String^ folderPath = docPath + L"\\FFB_History";
		if (!System::IO::Directory::Exists(folderPath)) System::IO::Directory::CreateDirectory(folderPath);

		System::String^ timeStamp = DateTime::Now.ToString("yyyyMMdd_HHmmss");
		System::String^ slipPath = folderPath + L"\\Slip_" + timeStamp + L".png";
		System::String^ imgPath = folderPath + L"\\Image_" + timeStamp + L".png";
		System::String^ rawImgPath = folderPath + L"\\RawImage_" + timeStamp + L".jpg";
		System::String^ jsonPath = folderPath + L"\\Data_" + timeStamp + L".json"; 
		System::String^ txtPath = folderPath + L"\\Data_" + timeStamp + L".txt";
		System::String^ csvPath = folderPath + L"\\HistoryData.csv";

		// Print Receipt Graphics Generation
	int slipWidth = 400; int slipHeight = 500;
	System::Drawing::Bitmap^ bmpSlip = gcnew System::Drawing::Bitmap(slipWidth, slipHeight);
	System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmpSlip);
	g->Clear(System::Drawing::Color::White);
	g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

	System::Drawing::Font^ fTitle = gcnew System::Drawing::Font(L"Tahoma", 16, System::Drawing::FontStyle::Bold);
	System::Drawing::Font^ fSub = gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular);
	System::Drawing::Font^ fMono = gcnew System::Drawing::Font(L"Courier New", 11, System::Drawing::FontStyle::Bold);

	g->DrawString(L"SMART PALM STATION", fTitle, System::Drawing::Brushes::Black, 80, 20);
	g->DrawString(L"WEIGHING TICKET", fSub, System::Drawing::Brushes::Gray, 130, 50);
	g->DrawString(L"------------------------------------------------", fSub, System::Drawing::Brushes::Black, 10, 70);

	System::String^ dateStr = DateTime::Now.ToString(L"dd/MM/yyyy HH:mm");
	g->DrawString(L"DATE: " + dateStr, fSub, System::Drawing::Brushes::Black, 15, 90);

	g->DrawString(L"GROSS WT: " + txtWeightIn->Text + L" KG", fSub, System::Drawing::Brushes::Black, 15, 120);
	g->DrawString(L"TARE WT:  " + txtWeightOut->Text + L" KG", fSub, System::Drawing::Brushes::Black, 15, 140);
	g->DrawString(L"NET WT:   " + curNetWeight.ToString() + L" KG", fMono, System::Drawing::Brushes::DarkGreen, 15, 165);

	g->DrawString(L"------------------------------------------------", fSub, System::Drawing::Brushes::Black, 10, 190);

	g->DrawString(L"BASE RATE:   " + currentBasePrice.ToString(L"F2"), fSub, System::Drawing::Brushes::Black, 15, 210);
	g->DrawString(L"QUALITY DED: -" + curTotalDeduction.ToString(L"F2"), fSub, System::Drawing::Brushes::Red, 15, 230);
	g->DrawString(L"NET RATE:    " + curNetRate.ToString(L"F2"), fMono, System::Drawing::Brushes::Black, 15, 255);

	g->DrawString(L"------------------------------------------------", fSub, System::Drawing::Brushes::Black, 10, 280);

	g->DrawString(L"TOTAL PAYABLE (THB):", fTitle, System::Drawing::Brushes::Black, 15, 310);
	g->DrawString(L"THB " + curTotalPrice.ToString(L"N2"), gcnew System::Drawing::Font(L"Tahoma", 18, System::Drawing::FontStyle::Bold), System::Drawing::Brushes::DarkGreen, 15, 340);

		bmpSlip->Save(slipPath);
		delete g; // Release graphics memory

		// Instancing Preview Window
	System::Windows::Forms::Form^ previewForm = gcnew System::Windows::Forms::Form();
	previewForm->Text = L"Print Document Preview";
	previewForm->Size = System::Drawing::Size(450, 650);
	previewForm->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
	previewForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	previewForm->MaximizeBox = false;
	previewForm->MinimizeBox = false;
	previewForm->BackColor = System::Drawing::Color::FromArgb(30, 30, 30); // พื้นหลังสีเทาเข้มเหมือนใน React

	// 3.1 กล่องแสดงรูปใบเสร็จ
	System::Windows::Forms::PictureBox^ picPreview = gcnew System::Windows::Forms::PictureBox();
	picPreview->Image = bmpSlip;
	picPreview->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	picPreview->Location = System::Drawing::Point(15, 15);
	picPreview->Size = System::Drawing::Size(400, 500);
	picPreview->BackColor = System::Drawing::Color::White;
	previewForm->Controls->Add(picPreview);

	// 3.2 ปุ่ม Print
	System::Windows::Forms::Button^ btnPrint = gcnew System::Windows::Forms::Button();
	btnPrint->Text = L"🖨️ Print to COM1";
	btnPrint->Size = System::Drawing::Size(250, 45);
	btnPrint->Location = System::Drawing::Point(15, 530);
	btnPrint->BackColor = System::Drawing::Color::FromArgb(64, 64, 64);
	btnPrint->ForeColor = System::Drawing::Color::White;
	btnPrint->Font = gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Bold);
	btnPrint->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	previewForm->Controls->Add(btnPrint);
	currentPrintSlipPath = slipPath; // จำที่อยู่ใบเสร็จไว้
	btnPrint->Click += gcnew System::EventHandler(this, &MainForm::btnPrintPopup_Click); // ผูกสายไฟสั่งปริ้นท์!

	// 3.3 ปุ่ม Close
	System::Windows::Forms::Button^ btnClose = gcnew System::Windows::Forms::Button();
	btnClose->Text = L"Close";
	btnClose->Size = System::Drawing::Size(135, 45);
	btnClose->Location = System::Drawing::Point(280, 530);
	btnClose->BackColor = System::Drawing::Color::DarkRed;
	btnClose->ForeColor = System::Drawing::Color::White;
	btnClose->Font = gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Bold);
	btnClose->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	btnClose->DialogResult = System::Windows::Forms::DialogResult::OK; // ตั้งให้ปิดฟอร์มเมื่อกด
	previewForm->Controls->Add(btnClose);

	// 3.4 โชว์หน้าต่างขึ้นมา (โปรแกรมจะหยุดรอจนกว่าผู้ใช้จะกดปุ่ม Close)
	previewForm->ShowDialog();

	delete bmpSlip; // ปิดหน้าต่างแล้วค่อยคืนหน่วยความจำรูปภาพ
	// ========================================================

	// 4. 📁 เซฟรูปภาพต้นฉบับ และไฟล์พิกัดกรอบ AI (.txt)
	if (currentOriginalImgPath != L"") {
		System::IO::File::Copy(currentOriginalImgPath, imgPath, true);
		System::IO::File::WriteAllText(txtPath, currentBoxData, System::Text::Encoding::UTF8);
		// 🌟 เซฟภาพต้นฉบับ (raw) และไฟล์พิกัด AI
		if (cmbCameraSource->SelectedIndex != 0) {
			rawImgPath = imgPath; // currentOriginalImgPath is a raw frame for Video
			jsonPath = txtPath;   // jsonPath is deprecated, use the serialized text path directly
		} else {
			rawImgPath = imgPath; 
			jsonPath = L"None";
		}
	}
	else {
		imgPath = L"None"; rawImgPath = L"None"; jsonPath = L"None"; txtPath = L"None";
	}

	// 5. 💾 บันทึกประวัติลงไฟล์ CSV (เพิ่มคอลัมน์ rawImgPath, jsonPath)
	System::String^ csvData = dateStr + L"," + curNetWeight + L"," + curNetRate + L"," + curTotalPrice + L"," + curEmpty + L"," + curOverripe + L"," + curRipe + L"," + curRotten + L"," + curUnderripe + L"," + curUnripe + L"," + imgPath + L"," + slipPath + L"," + txtPath + L"," + rawImgPath + L"," + jsonPath + L"\n";

		if (!System::IO::File::Exists(csvPath)) {
			System::IO::File::WriteAllText(csvPath, L"Date,NetWeight,Rate,TotalPrice,Empty,Overripe,Ripe,Rotten,Underripe,Unripe,ImagePath,SlipPath,TxtPath,RawImagePath,JsonPath\n", System::Text::Encoding::UTF8);
		}
		System::IO::File::AppendAllText(csvPath, csvData, System::Text::Encoding::UTF8);

		// Post-Render Cleanup
	btnClearNew_Click(nullptr, nullptr);
}
private: System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
}
	// Read History Data directly from Flat CSV File
	private: void LoadDashboardData() {
		System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
		System::String^ csvPath = docPath + L"\\FFB_History\\HistoryData.csv";

		dgvHistory->Rows->Clear();
		double sumWeight = 0; double sumPayout = 0;
		int sEmpty = 0, sOverripe = 0, sRipe = 0, sRotten = 0, sUnderripe = 0, sUnripe = 0;

		// Rebuild history records
		if (System::IO::File::Exists(csvPath)) {
			array<System::String^>^ lines = System::IO::File::ReadAllLines(csvPath, System::Text::Encoding::UTF8);

			for (int i = 1; i < lines->Length; i++) { // Skip header offset i=1
				if (System::String::IsNullOrWhiteSpace(lines[i])) continue;
				array<System::String^>^ cols = lines[i]->Split(',');

				if (cols->Length >= 13) {
					System::String^ dateStr = cols[0];
					double weight = Convert::ToDouble(cols[1]);
					double rate = Convert::ToDouble(cols[2]);
					double total = Convert::ToDouble(cols[3]);

					sEmpty += Convert::ToInt32(cols[4]); sOverripe += Convert::ToInt32(cols[5]);
					sRipe += Convert::ToInt32(cols[6]); sRotten += Convert::ToInt32(cols[7]);
					sUnderripe += Convert::ToInt32(cols[8]); sUnripe += Convert::ToInt32(cols[9]);
					System::String^ slipPath = cols[11];

					sumWeight += weight;
					sumPayout += total;

					System::String^ txId = L"TX-" + i.ToString(L"D4");

					int rowIndex = dgvHistory->Rows->Add(txId, dateStr, weight.ToString(L"N0"), rate.ToString(L"F2"), total.ToString(L"N2"));
					
					// Encode row-specific payload object
					System::String^ rawPath = (cols->Length >= 14) ? cols[13]->Trim()->Replace(L"\"", L"") : cols[10]->Trim()->Replace(L"\"", L"");
					System::String^ jsonPath = (cols->Length >= 15) ? cols[14]->Trim()->Replace(L"\"", L"") : L"None";
					System::String^ rowSecretData = cols[11] + L"|" + cols[4] + L"|" + cols[5] + L"|" + cols[6] + L"|" + cols[7] + L"|" + cols[8] + L"|" + cols[9] + L"|" + rawPath + L"|" + cols[12] + L"|" + jsonPath;
					dgvHistory->Rows[rowIndex]->Tag = rowSecretData;
				}
			}
		}

		// Calculate Daily Dashboard Metrics
		double avgRate = 0;
		if (sumWeight > 0) avgRate = sumPayout / sumWeight;

		lblDashTotalWeight->Text = sumWeight.ToString(L"N0");
		lblDashTotalPayout->Text = sumPayout.ToString(L"N2");
		lblDashAvgRate->Text = avgRate.ToString(L"F2");

		// Display Aggregate Class Distributions on Graph
		chartDashQuality->Series[0]->Points->Clear();

		// 1. สุก
		chartDashQuality->Series[0]->Points->AddXY(L"สุก", sRipe);
		chartDashQuality->Series[0]->Points[0]->Color = Color::LimeGreen;
		chartDashQuality->Series[0]->Points[0]->LegendText = L"สุก: " + sRipe.ToString();
		if (sRipe > 0) { chartDashQuality->Series[0]->Points[0]->Label = L"สุก"; }
		else { chartDashQuality->Series[0]->Points[0]->Label = L" "; }

		// 2. ห่าม
		chartDashQuality->Series[0]->Points->AddXY(L"ห่าม", sUnderripe);
		chartDashQuality->Series[0]->Points[1]->Color = Color::Gold;
		chartDashQuality->Series[0]->Points[1]->LegendText = L"ห่าม: " + sUnderripe.ToString();
		if (sUnderripe > 0) { chartDashQuality->Series[0]->Points[1]->Label = L"ห่าม"; }
		else { chartDashQuality->Series[0]->Points[1]->Label = L" "; }

		// 3. สุกเกิน
		chartDashQuality->Series[0]->Points->AddXY(L"สุกเกิน", sOverripe);
		chartDashQuality->Series[0]->Points[2]->Color = Color::Orange;
		chartDashQuality->Series[0]->Points[2]->LegendText = L"สุกเกิน: " + sOverripe.ToString();
		if (sOverripe > 0) { chartDashQuality->Series[0]->Points[2]->Label = L"สุกเกิน"; }
		else { chartDashQuality->Series[0]->Points[2]->Label = L" "; }

		// 4. ดิบ
		chartDashQuality->Series[0]->Points->AddXY(L"ดิบ", sUnripe);
		chartDashQuality->Series[0]->Points[3]->Color = Color::DeepSkyBlue;
		chartDashQuality->Series[0]->Points[3]->LegendText = L"ดิบ: " + sUnripe.ToString();
		if (sUnripe > 0) { chartDashQuality->Series[0]->Points[3]->Label = L"ดิบ"; }
		else { chartDashQuality->Series[0]->Points[3]->Label = L" "; }

		// 5. ทะลายเปล่า
		chartDashQuality->Series[0]->Points->AddXY(L"ทะลายเปล่า", sEmpty);
		chartDashQuality->Series[0]->Points[4]->Color = Color::LightCoral;
		chartDashQuality->Series[0]->Points[4]->LegendText = L"ทะลายเปล่า: " + sEmpty.ToString();
		if (sEmpty > 0) { chartDashQuality->Series[0]->Points[4]->Label = L"ทะลายเปล่า"; }
		else { chartDashQuality->Series[0]->Points[4]->Label = L" "; }

		// 6. เน่า
		chartDashQuality->Series[0]->Points->AddXY(L"เน่า", sRotten);
		chartDashQuality->Series[0]->Points[5]->Color = Color::Silver;
		chartDashQuality->Series[0]->Points[5]->LegendText = L"เน่า: " + sRotten.ToString();
		if (sRotten > 0) { chartDashQuality->Series[0]->Points[5]->Label = L"เน่า"; }
		else { chartDashQuality->Series[0]->Points[5]->Label = L" "; }
	}
	// Filter Dashboard Records by Date Range
	private: void SearchDashboardData(System::DateTime startDate, System::DateTime endDate) {
		System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
		System::String^ csvPath = docPath + L"\\FFB_History\\HistoryData.csv";

		dgvHistory->Rows->Clear();
		double sumWeight = 0; double sumPayout = 0;
		int sEmpty = 0, sOverripe = 0, sRipe = 0, sRotten = 0, sUnderripe = 0, sUnripe = 0;

		if (System::IO::File::Exists(csvPath)) {
			array<System::String^>^ lines = System::IO::File::ReadAllLines(csvPath, System::Text::Encoding::UTF8);

			for (int i = 1; i < lines->Length; i++) {
				if (System::String::IsNullOrWhiteSpace(lines[i])) continue;
				array<System::String^>^ cols = lines[i]->Split(',');

				if (cols->Length >= 13) {
					System::String^ dateStr = cols[0];
					System::DateTime rowDate;

					array<System::String^>^ expectedFormats = { L"dd/MM/yyyy HH:mm", L"dd/MM/yyyy H:mm", L"d/MM/yyyy HH:mm", L"d/MM/yyyy H:mm" };
					if (System::DateTime::TryParseExact(dateStr, expectedFormats, System::Globalization::CultureInfo::InvariantCulture, System::Globalization::DateTimeStyles::None, rowDate)) {

						// Filter by Date Match
						if (rowDate.Date >= startDate.Date && rowDate.Date <= endDate.Date) {

							double weight = Convert::ToDouble(cols[1]);
							double rate = Convert::ToDouble(cols[2]);
							double total = Convert::ToDouble(cols[3]);

							sEmpty += Convert::ToInt32(cols[4]); sOverripe += Convert::ToInt32(cols[5]);
							sRipe += Convert::ToInt32(cols[6]); sRotten += Convert::ToInt32(cols[7]);
							sUnderripe += Convert::ToInt32(cols[8]); sUnripe += Convert::ToInt32(cols[9]);
							System::String^ slipPath = cols[11];

							sumWeight += weight;
							sumPayout += total;

							System::String^ txId = L"TX-" + i.ToString(L"D4");
							int rowIndex = dgvHistory->Rows->Add(txId, dateStr, weight.ToString(L"N0"), rate.ToString(L"F2"), total.ToString(L"N2"));

							System::String^ rawPath2 = (cols->Length >= 14) ? cols[13]->Trim()->Replace(L"\"", L"") : cols[10]->Trim()->Replace(L"\"", L"");
							System::String^ jsonPath2 = (cols->Length >= 15) ? cols[14]->Trim()->Replace(L"\"", L"") : L"None";
							System::String^ rowSecretData = cols[11] + L"|" + cols[4] + L"|" + cols[5] + L"|" + cols[6] + L"|" + cols[7] + L"|" + cols[8] + L"|" + cols[9] + L"|" + rawPath2 + L"|" + cols[12] + L"|" + jsonPath2;
							dgvHistory->Rows[rowIndex]->Tag = rowSecretData;
						}
					}
				}
			}
		}

		// Load Aggregated Search Result Graph
		chartDashQuality->Series[0]->Points->Clear();

		chartDashQuality->Series[0]->Points->AddXY(L"สุก", sRipe);
		chartDashQuality->Series[0]->Points[0]->Color = Color::LimeGreen;
		chartDashQuality->Series[0]->Points[0]->LegendText = L"สุก: " + sRipe.ToString();
		chartDashQuality->Series[0]->Points[0]->Label = (sRipe > 0) ? L"สุก" : L" ";

		chartDashQuality->Series[0]->Points->AddXY(L"ห่าม", sUnderripe);
		chartDashQuality->Series[0]->Points[1]->Color = Color::Gold;
		chartDashQuality->Series[0]->Points[1]->LegendText = L"ห่าม: " + sUnderripe.ToString();
		chartDashQuality->Series[0]->Points[1]->Label = (sUnderripe > 0) ? L"ห่าม" : L" ";

		chartDashQuality->Series[0]->Points->AddXY(L"สุกเกิน", sOverripe);
		chartDashQuality->Series[0]->Points[2]->Color = Color::Orange;
		chartDashQuality->Series[0]->Points[2]->LegendText = L"สุกเกิน: " + sOverripe.ToString();
		chartDashQuality->Series[0]->Points[2]->Label = (sOverripe > 0) ? L"สุกเกิน" : L" ";

		chartDashQuality->Series[0]->Points->AddXY(L"ดิบ", sUnripe);
		chartDashQuality->Series[0]->Points[3]->Color = Color::DeepSkyBlue;
		chartDashQuality->Series[0]->Points[3]->LegendText = L"ดิบ: " + sUnripe.ToString();
		chartDashQuality->Series[0]->Points[3]->Label = (sUnripe > 0) ? L"ดิบ" : L" ";

		chartDashQuality->Series[0]->Points->AddXY(L"ทะลายเปล่า", sEmpty);
		chartDashQuality->Series[0]->Points[4]->Color = Color::LightCoral;
		chartDashQuality->Series[0]->Points[4]->LegendText = L"ทะลายเปล่า: " + sEmpty.ToString();
		chartDashQuality->Series[0]->Points[4]->Label = (sEmpty > 0) ? L"ทะลายเปล่า" : L" ";

		chartDashQuality->Series[0]->Points->AddXY(L"เน่า", sRotten);
		chartDashQuality->Series[0]->Points[5]->Color = Color::Silver;
		chartDashQuality->Series[0]->Points[5]->LegendText = L"เน่า: " + sRotten.ToString();
		chartDashQuality->Series[0]->Points[5]->Label = (sRotten > 0) ? L"เน่า" : L" ";
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		pnlDashboard->Visible = false;
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		pnlDashboard->Visible = true;
		pnlDashboard->BringToFront();
		LoadDashboardData();
	}
private: System::Void dgvHistory_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
	// Guard invalid interactions with table headers
	if (e->RowIndex >= 0) {

		// Verify Print Trigger Activity
		if (dgvHistory->Columns[e->ColumnIndex]->GetType() == System::Windows::Forms::DataGridViewButtonColumn::typeid) {

			System::String^ secretData = safe_cast<System::String^>(dgvHistory->Rows[e->RowIndex]->Tag);

			if (secretData != nullptr) {
				System::String^ slipPath = secretData->Split('|')[0];

				if (System::IO::File::Exists(slipPath)) {

					// Initialize External Component Print Preview
					System::Windows::Forms::Form^ previewForm = gcnew System::Windows::Forms::Form();
					previewForm->Text = L"Print Document Preview - " + dgvHistory->Rows[e->RowIndex]->Cells[0]->Value->ToString();
					previewForm->Size = System::Drawing::Size(450, 650);
					previewForm->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
					previewForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
					previewForm->MaximizeBox = false;
					previewForm->MinimizeBox = false;
					previewForm->BackColor = System::Drawing::Color::FromArgb(30, 30, 30);

					System::Windows::Forms::PictureBox^ picPreview = gcnew System::Windows::Forms::PictureBox();
					picPreview->ImageLocation = slipPath;
					picPreview->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
					picPreview->Location = System::Drawing::Point(15, 15);
					picPreview->Size = System::Drawing::Size(400, 500);
					picPreview->BackColor = System::Drawing::Color::White;
					previewForm->Controls->Add(picPreview);

					System::Windows::Forms::Button^ btnPrint = gcnew System::Windows::Forms::Button();
					btnPrint->Text = L"🖨️ Print Document";
					btnPrint->Size = System::Drawing::Size(250, 45);
					btnPrint->Location = System::Drawing::Point(15, 530);
					btnPrint->BackColor = System::Drawing::Color::FromArgb(64, 64, 64);
					btnPrint->ForeColor = System::Drawing::Color::White;
					btnPrint->Font = gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Bold);
					btnPrint->FlatStyle = System::Windows::Forms::FlatStyle::Flat;

					currentPrintSlipPath = slipPath;
					btnPrint->Click += gcnew System::EventHandler(this, &MainForm::btnPrintPopup_Click);

					previewForm->Controls->Add(btnPrint);

					System::Windows::Forms::Button^ btnClose = gcnew System::Windows::Forms::Button();
					btnClose->Text = L"Close";
					btnClose->Size = System::Drawing::Size(135, 45);
					btnClose->Location = System::Drawing::Point(280, 530);
					btnClose->BackColor = System::Drawing::Color::DarkRed;
					btnClose->ForeColor = System::Drawing::Color::White;
					btnClose->Font = gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Bold);
					btnClose->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
					btnClose->DialogResult = System::Windows::Forms::DialogResult::OK;
					previewForm->Controls->Add(btnClose);

					previewForm->ShowDialog();
				}
				else {
					MessageBox::Show(L"ไม่พบไฟล์รูปใบเสร็จครับ อาจถูกลบหรือย้ายไปแล้ว", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
		}
	}
}
private: System::Void dgvHistory_CellClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
	// เช็คว่าไม่ได้คลิกโดนหัวตาราง
	if (e->RowIndex >= 0) {
		System::String^ secretData = safe_cast<System::String^>(dgvHistory->Rows[e->RowIndex]->Tag);
		if (secretData != nullptr) {
			array<System::String^>^ parts = secretData->Split('|');
			if (parts->Length >= 7) {
				// 1. ดึงข้อมูลของบรรทัดที่ถูกคลิกออกมา
				int rEmpty = Convert::ToInt32(parts[1]); int rOverripe = Convert::ToInt32(parts[2]);
				int rRipe = Convert::ToInt32(parts[3]);  int rRotten = Convert::ToInt32(parts[4]);
				int rUnderripe = Convert::ToInt32(parts[5]); int rUnripe = Convert::ToInt32(parts[6]);

				// 2. สั่งวาดกราฟโดนัทใหม่ "เฉพาะข้อมูลของบรรทัดนี้" ทันที!
				chartDashQuality->Series[0]->Points->Clear();

				chartDashQuality->Series[0]->Points->AddXY(L"สุก", rRipe);
				chartDashQuality->Series[0]->Points[0]->Color = Color::LimeGreen;
				chartDashQuality->Series[0]->Points[0]->LegendText = L"สุก: " + rRipe.ToString();
				chartDashQuality->Series[0]->Points[0]->Label = (rRipe > 0) ? L"สุก" : L" ";

				chartDashQuality->Series[0]->Points->AddXY(L"ห่าม", rUnderripe);
				chartDashQuality->Series[0]->Points[1]->Color = Color::Gold;
				chartDashQuality->Series[0]->Points[1]->LegendText = L"ห่าม: " + rUnderripe.ToString();
				chartDashQuality->Series[0]->Points[1]->Label = (rUnderripe > 0) ? L"ห่าม" : L" ";

				chartDashQuality->Series[0]->Points->AddXY(L"สุกเกิน", rOverripe);
				chartDashQuality->Series[0]->Points[2]->Color = Color::Orange;
				chartDashQuality->Series[0]->Points[2]->LegendText = L"สุกเกิน: " + rOverripe.ToString();
				chartDashQuality->Series[0]->Points[2]->Label = (rOverripe > 0) ? L"สุกเกิน" : L" ";

				chartDashQuality->Series[0]->Points->AddXY(L"ดิบ", rUnripe);
				chartDashQuality->Series[0]->Points[3]->Color = Color::DeepSkyBlue;
				chartDashQuality->Series[0]->Points[3]->LegendText = L"ดิบ: " + rUnripe.ToString();
				chartDashQuality->Series[0]->Points[3]->Label = (rUnripe > 0) ? L"ดิบ" : L" ";

				chartDashQuality->Series[0]->Points->AddXY(L"ทะลายเปล่า", rEmpty);
				chartDashQuality->Series[0]->Points[4]->Color = Color::LightCoral;
				chartDashQuality->Series[0]->Points[4]->LegendText = L"ทะลายเปล่า: " + rEmpty.ToString();
				chartDashQuality->Series[0]->Points[4]->Label = (rEmpty > 0) ? L"ทะลายเปล่า" : L" ";

				chartDashQuality->Series[0]->Points->AddXY(L"เน่า", rRotten);
				chartDashQuality->Series[0]->Points[5]->Color = Color::Silver;
				chartDashQuality->Series[0]->Points[5]->LegendText = L"เน่า: " + rRotten.ToString();
				chartDashQuality->Series[0]->Points[5]->Label = (rRotten > 0) ? L"เน่า" : L" ";
			}
		}
	}
}
private: System::Void btnResetFilter_Click(System::Object^ sender, System::EventArgs^ e) {
	// Re-trigger root dataloader to reload everything into standard overview mode
	LoadDashboardData();
}

	// Display Native Environment Print Spooler Dialogue
	private: System::Void btnPrintPopup_Click(System::Object^ sender, System::EventArgs^ e) {
		System::Windows::Forms::PrintDialog^ printDlg = gcnew System::Windows::Forms::PrintDialog();
		System::Drawing::Printing::PrintDocument^ printDoc = gcnew System::Drawing::Printing::PrintDocument();

		printDoc->PrintPage += gcnew System::Drawing::Printing::PrintPageEventHandler(this, &MainForm::PrintReceiptPage);
		printDlg->Document = printDoc;

		if (printDlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			try {
				printDoc->Print();
			}
			catch (...) {
				MessageBox::Show(L"เกิดข้อผิดพลาดในการปริ้นท์ กรุณาตรวจสอบการเชื่อมต่อเครื่องปริ้นท์ครับ", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
	}

	// Graphic construction callback for Print Spooler hook
	private: System::Void PrintReceiptPage(System::Object^ sender, System::Drawing::Printing::PrintPageEventArgs^ e) {
		if (System::IO::File::Exists(currentPrintSlipPath)) {
			System::Drawing::Image^ slipImg = System::Drawing::Image::FromFile(currentPrintSlipPath);
			// Render flat graphic against the upper-left print origin
			e->Graphics->DrawImage(slipImg, System::Drawing::Point(0, 0));
			delete slipImg;
		}
	}

private: System::Void btnDeleteAll_Click(System::Object^ sender, System::EventArgs^ e) {
	if (MessageBox::Show(L"คุณต้องการลบประวัติการรับซื้อ 'ทั้งหมด' ใช่หรือไม่?\n(ข้อมูลจะไม่สามารถกู้คืนได้)", L"ยืนยันการลบ", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes) {

		System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
		System::String^ csvPath = docPath + L"\\FFB_History\\HistoryData.csv";

		if (System::IO::File::Exists(csvPath)) {
			System::IO::File::Delete(csvPath);
		}

		LoadDashboardData();
		MessageBox::Show(L"ลบประวัติทั้งหมดเรียบร้อยแล้ว", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
}
private: System::Void btnDeleteRow_Click(System::Object^ sender, System::EventArgs^ e) {
	// Guard against null selection
	if (dgvHistory->CurrentRow == nullptr) {
		MessageBox::Show(L"กรุณาคลิกเลือกรายการที่ต้องการลบในตารางก่อนครับ", L"Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	int rowIndex = dgvHistory->CurrentRow->Index;
	System::String^ txId = dgvHistory->Rows[rowIndex]->Cells[0]->Value->ToString();

	if (MessageBox::Show(L"คุณต้องการลบรายการ [" + txId + L"] ใช่หรือไม่?", L"ยืนยันการลบ", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {

		System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
		System::String^ csvPath = docPath + L"\\FFB_History\\HistoryData.csv";

		if (System::IO::File::Exists(csvPath)) {
			// Pull lines and explicitly rebuild CSV omitting deleted row index
			array<System::String^>^ lines = System::IO::File::ReadAllLines(csvPath, System::Text::Encoding::UTF8);
			System::Collections::Generic::List<System::String^>^ newLines = gcnew System::Collections::Generic::List<System::String^>();

			newLines->Add(lines[0]); // Persist CSV header

			int currentDataIndex = 0;
			for (int i = 1; i < lines->Length; i++) {
				if (System::String::IsNullOrWhiteSpace(lines[i])) continue;

				if (currentDataIndex != rowIndex) {
					newLines->Add(lines[i]);
				}
				currentDataIndex++;
			}

			System::IO::File::WriteAllLines(csvPath, newLines->ToArray(), System::Text::Encoding::UTF8);
		}

		LoadDashboardData();
	}
}
private: System::Void btnFilterDate_Click(System::Object^ sender, System::EventArgs^ e) {
	System::DateTime startDate = dtpStartDate->Value.Date;
	System::DateTime endDate = dtpEndDate->Value.Date;

	// Logical sanity check on input values
	if (startDate > endDate) {
		MessageBox::Show(L"กรุณาเลือกช่วงวันที่ให้ถูกต้องครับ (วันที่เริ่มต้นต้องไม่มากกว่าวันที่สิ้นสุด)", L"Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	SearchDashboardData(startDate, endDate);
}
	// Transmit active GUI preferences to temporary configuration pipeline
	private: void UpdateDisplayConfig() {
		try {
			System::String^ boxState = L"1";
			System::String^ maskState = L"1";

			// Determine which overlay panel is currently visible to grab the appropriate button states
			if (pnlAIPreview->Visible) {
				boxState = chkHistBox->Checked ? L"1" : L"0";
				maskState = chkHistMask->Checked ? L"1" : L"0";
			}
			else {
				// Capture root operational settings
				boxState = chkShowBox->Checked ? L"1" : L"0";
				maskState = chkShowMask->Checked ? L"1" : L"0";
			}

			int filterId = -1;
			if (pnlAIPreview->Visible && cmbAIFilter->SelectedIndex > 0) {
				filterId = cmbAIFilter->SelectedIndex - 1;
			}

			System::String^ configText = boxState + L"," + maskState + L"," + filterId.ToString();
			System::IO::File::WriteAllText(L"C:\\projectsky\\display_config.txt", configText);
		}
		catch (...) {}
	}

	private: System::Void chkShowBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		UpdateDisplayConfig();
		if (pnlAIPreview->Visible) DrawAIBoundingBoxes(-1);
		else RedrawMainDataEntryImage();
	}

	private: System::Void chkShowMask_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		UpdateDisplayConfig();
		if (pnlAIPreview->Visible) DrawAIBoundingBoxes(-1);
		else RedrawMainDataEntryImage();
	}

	// Redraw primary UI display payload upon layer toggling
	private: void RedrawMainDataEntryImage() {
		if (cmbCameraSource->SelectedIndex != 0) return; // Ignore on Video streams
		if (currentOriginalImgPath == L"" || !System::IO::File::Exists(currentOriginalImgPath)) return;
		
		this->Cursor = System::Windows::Forms::Cursors::WaitCursor;
		System::String^ tempImgPath = currentOriginalImgPath;
		std::string imgPath = msclr::interop::marshal_as<std::string>(tempImgPath);
		std::string modelPath = "C:\\projectsky\\Final300.onnx";
		cv::Mat resultImg;
		std::string dummyBoxStr = "";
		int tE = 0, tO = 0, tR = 0, tRot = 0, tU = 0, tUn = 0;

		bool success = RunYOLOv8_AI(imgPath, modelPath, resultImg, tE, tO, tR, tRot, tU, tUn, dummyBoxStr);

		this->Cursor = System::Windows::Forms::Cursors::Default;

		if (success && !resultImg.empty()) {
			if (picUploadAreaNew->Image != nullptr) delete picUploadAreaNew->Image;
			picUploadAreaNew->Image = MatToBitmap(resultImg);
		}
	}

	// Plot JSON or Document History Records Over Raw File Stream
	private: void DrawSavedDetections(System::String^ imgPath, System::String^ dataPath, int filterClassId) {
		if (!System::IO::File::Exists(imgPath) || !System::IO::File::Exists(dataPath)) return;

		this->Cursor = System::Windows::Forms::Cursors::WaitCursor;

		// Construct OpenCV Matrix
		std::string sImgPath = msclr::interop::marshal_as<std::string>(imgPath);
		cv::Mat img = cv::imread(sImgPath);
		if (img.empty()) {
			this->Cursor = System::Windows::Forms::Cursors::Default;
			return;
		}

		// Read configuration file filters
		bool drawBox = chkHistBox->Checked;
		bool drawMask = chkHistMask->Checked;

		// Reinitialize BGR local array mappings aligning with `YoloEngine` source truth
		std::vector<cv::Scalar> classColors = {
			cv::Scalar(128, 128, 240), // 0: Empty (LightCoral)
			cv::Scalar(0, 165, 255),   // 1: Overripe (Orange)
			cv::Scalar(50, 205, 50),   // 2: Ripe (LimeGreen)
			cv::Scalar(192, 192, 192), // 3: Rotten (Silver)
			cv::Scalar(0, 215, 255),   // 4: Underripe (Gold)
			cv::Scalar(255, 191, 0)    // 5: Unripe (DeepSkyBlue)
		};
		std::vector<std::string> classNames = { "empty", "overripe", "ripe", "rotten", "underripe", "unripe" };

		// Parse serialized structural definitions
		array<System::String^>^ lines = System::IO::File::ReadAllLines(dataPath, System::Text::Encoding::UTF8);
		
		for each (System::String^ line in lines) {
			array<System::String^>^ parts = line->Split(';');
			if (parts->Length >= 4) {
				int objId = Convert::ToInt32(parts[0]);
				int cls = Convert::ToInt32(parts[1]);
				
				// Optional filtering by isolated classification ID
				if (filterClassId != -1 && cls != filterClassId) continue;

				float conf = Convert::ToSingle(parts[2]);
				array<System::String^>^ boxCoords = parts[3]->Split(',');

				if (boxCoords->Length == 4) {
					int x1 = Convert::ToInt32(boxCoords[0]);
					int y1 = Convert::ToInt32(boxCoords[1]);
					int x2 = Convert::ToInt32(boxCoords[2]);
					int y2 = Convert::ToInt32(boxCoords[3]);

					cv::Scalar color = (cls >= 0 && cls < 6) ? classColors[cls] : cv::Scalar(255, 255, 255);
					std::string name = (cls >= 0 && cls < 6) ? classNames[cls] : "unknown";

					// Draw corresponding segmentation polymask
					if (drawMask && parts->Length >= 5 && parts[4]->Length > 0) {
						array<System::String^>^ maskCoords = parts[4]->Split(',');
						std::vector<cv::Point> pts;
						for (int i = 0; i < maskCoords->Length; i+=2) {
							if (i+1 < maskCoords->Length) {
								pts.push_back(cv::Point(Convert::ToInt32(maskCoords[i]), Convert::ToInt32(maskCoords[i+1])));
							}
						}
						
						if (!pts.empty()) {
							// Blend transparent fill colors in memory
							cv::Mat overlay;
							img.copyTo(overlay);
							std::vector<std::vector<cv::Point>> polys = { pts };
							cv::fillPoly(overlay, polys, color);
							cv::addWeighted(overlay, 0.4, img, 0.6, 0, img);
						}
					}

					// Outline bounding box rendering
					if (drawBox) {
						cv::rectangle(img, cv::Point(x1, y1), cv::Point(x2, y2), color, 2);
						
						// Render label rectangle and overlay text object
						std::string label = "id:" + std::to_string(objId) + " " + name + " " + std::to_string(conf).substr(0, 4);
						int baseLine = 0;
						cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
						y1 = max(y1, labelSize.height);
						cv::rectangle(img, cv::Point(x1, y1 - labelSize.height), cv::Point(x1 + labelSize.width, y1 + baseLine), color, cv::FILLED);
						cv::putText(img, label, cv::Point(x1, y1), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
					}
				}
			}
		}

		this->Cursor = System::Windows::Forms::Cursors::Default;

		if (picAI->Image != nullptr) delete picAI->Image;
		picAI->Image = MatToBitmap(img);
	}


	// Dynamically Redraw AI Boxes for Active Preview Rendering
	private: void DrawAIBoundingBoxes(int filterClassId) {
		if (!System::IO::File::Exists(currentPreviewImgPath)) return;

		UpdateDisplayConfig();

		this->Cursor = System::Windows::Forms::Cursors::WaitCursor;

		System::String^ tempImgPath = currentPreviewImgPath;
		std::string imgPath = msclr::interop::marshal_as<std::string>(tempImgPath);
		std::string modelPath = "C:\\projectsky\\Final300.onnx";
		cv::Mat resultImg;
		std::string dummyBoxStr = "";
		int tE = 0, tO = 0, tR = 0, tRot = 0, tU = 0, tUn = 0;

		bool success = RunYOLOv8_AI(imgPath, modelPath, resultImg, tE, tO, tR, tRot, tU, tUn, dummyBoxStr);

		this->Cursor = System::Windows::Forms::Cursors::Default;

		if (success && !resultImg.empty()) {
			if (picAI->Image != nullptr) delete picAI->Image;
			picAI->Image = MatToBitmap(resultImg);
		}
		else {
			if (picAI->Image != nullptr) delete picAI->Image;
			picAI->Image = System::Drawing::Image::FromFile(currentPreviewImgPath);
		}
	}

	// Pad UI boundaries into uniform dimensions expected by YOLO standard
	private: System::Drawing::Image^ CreateLetterboxImage(System::Drawing::Image^ originalImg, int targetW, int targetH) {
		float scale = System::Math::Min((float)targetW / originalImg->Width, (float)targetH / originalImg->Height);
		int newW = (int)(originalImg->Width * scale);
		int newH = (int)(originalImg->Height * scale);

		System::Drawing::Bitmap^ paddedBmp = gcnew System::Drawing::Bitmap(targetW, targetH);
		System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(paddedBmp);

		// Initialize letterbox background spacing matching training priors
		g->Clear(System::Drawing::Color::FromArgb(114, 114, 114));

		int x = (targetW - newW) / 2;
		int y = (targetH - newH) / 2;

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;
		g->DrawImage(originalImg, x, y, newW, newH);

		delete g;
		return paddedBmp;
	}

private: System::Void btnViewAI_Click(System::Object^ sender, System::EventArgs^ e) {
	if (dgvHistory->CurrentRow == nullptr) {
		MessageBox::Show(L"กรุณาคลิกเลือกรายการในตารางก่อนครับ", L"Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	System::String^ secretData = safe_cast<System::String^>(dgvHistory->Rows[dgvHistory->CurrentRow->Index]->Tag);
	array<System::String^>^ parts = secretData->Split('|');

	if (parts->Length >= 10) {
		// Strip artifacting strings from datareader payloads
		currentPreviewImgPath = parts[7]->Trim()->Replace("\"", "");
		currentPreviewTxtPath = parts[8]->Trim()->Replace("\"", "");
		
		System::String^ jsonPath = L"None";
		if (parts->Length >= 10) {
			jsonPath = parts[9]->Trim()->Replace("\"", "");
		}

		if (System::IO::File::Exists(currentPreviewImgPath)) {
			// Construct AI Result Modal Interface
			pnlAIPreview->Visible = true;
			pnlAIPreview->BringToFront();

			// Override filters to display all default objects on initialization
			cmbAIFilter->SelectedIndexChanged -= gcnew System::EventHandler(this, &MainForm::cmbAIFilter_SelectedIndexChanged);
			cmbAIFilter->SelectedIndex = 0;
			cmbAIFilter->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::cmbAIFilter_SelectedIndexChanged);
			
			// Resolve cached definitions or fallback to fresh model parsing
			if (jsonPath != L"None" && System::IO::File::Exists(jsonPath)) {
				picAI->Tag = jsonPath;
				DrawSavedDetections(currentPreviewImgPath, jsonPath, -1);
			} else {
				// Re-Execute Native Pipeline Code on Raw Images
				picAI->Tag = nullptr;
				DrawAIBoundingBoxes(-1);
			}

			// Render Receipt Visualizer
			System::String^ slipPath = parts[0]->Trim()->Replace("\"", "");
			if (System::IO::File::Exists(slipPath)) {
				picSlipPreview->ImageLocation = slipPath;
			}

			// Build Graphical Distribution Charts
			int rEmpty = Convert::ToInt32(parts[1]); int rOverripe = Convert::ToInt32(parts[2]);
			int rRipe = Convert::ToInt32(parts[3]);  int rRotten = Convert::ToInt32(parts[4]);
			int rUnderripe = Convert::ToInt32(parts[5]); int rUnripe = Convert::ToInt32(parts[6]);

			chartAIPreview->Series->Clear();
			chartAIPreview->Series->Add("Ripeness");
			chartAIPreview->Series["Ripeness"]->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Doughnut;
			chartAIPreview->ChartAreas[0]->BackColor = System::Drawing::Color::FromArgb(30, 30, 30);
			chartAIPreview->Series[0]->Points->Clear();

			chartAIPreview->Series[0]->Points->AddXY(L"สุก", rRipe); chartAIPreview->Series[0]->Points[0]->Color = Color::LimeGreen; chartAIPreview->Series[0]->Points[0]->LegendText = L"สุก: " + rRipe.ToString(); chartAIPreview->Series[0]->Points[0]->Label = (rRipe > 0) ? L"สุก" : L" ";

			chartAIPreview->Series[0]->Points->AddXY(L"ห่าม", rUnderripe); chartAIPreview->Series[0]->Points[1]->Color = Color::Gold; chartAIPreview->Series[0]->Points[1]->LegendText = L"ห่าม: " + rUnderripe.ToString(); chartAIPreview->Series[0]->Points[1]->Label = (rUnderripe > 0) ? L"ห่าม" : L" ";

			chartAIPreview->Series[0]->Points->AddXY(L"สุกเกิน", rOverripe); chartAIPreview->Series[0]->Points[2]->Color = Color::Orange; chartAIPreview->Series[0]->Points[2]->LegendText = L"สุกเกิน: " + rOverripe.ToString(); chartAIPreview->Series[0]->Points[2]->Label = (rOverripe > 0) ? L"สุกเกิน" : L" ";

			chartAIPreview->Series[0]->Points->AddXY(L"ดิบ", rUnripe); chartAIPreview->Series[0]->Points[3]->Color = Color::DeepSkyBlue; chartAIPreview->Series[0]->Points[3]->LegendText = L"ดิบ: " + rUnripe.ToString(); chartAIPreview->Series[0]->Points[3]->Label = (rUnripe > 0) ? L"ดิบ" : L" ";

			chartAIPreview->Series[0]->Points->AddXY(L"ทะลายเปล่า", rEmpty); chartAIPreview->Series[0]->Points[4]->Color = Color::LightCoral; chartAIPreview->Series[0]->Points[4]->LegendText = L"ทะลายเปล่า: " + rEmpty.ToString(); chartAIPreview->Series[0]->Points[4]->Label = (rEmpty > 0) ? L"ทะลายเปล่า" : L" ";

			chartAIPreview->Series[0]->Points->AddXY(L"เน่า", rRotten); chartAIPreview->Series[0]->Points[5]->Color = Color::Silver; chartAIPreview->Series[0]->Points[5]->LegendText = L"เน่า: " + rRotten.ToString(); chartAIPreview->Series[0]->Points[5]->Label = (rRotten > 0) ? L"เน่า" : L" ";

		}
		else {
			MessageBox::Show(L"ไม่พบไฟล์รูปภาพต้นฉบับครับ", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
}
private: System::Void cmbAIFilter_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	// Standardize array targets
	int targetClassId = cmbAIFilter->SelectedIndex - 1;

	if (pnlAIPreview->Visible) {
		if (picAI->Tag != nullptr) {
			System::String^ jsonPath = safe_cast<System::String^>(picAI->Tag);
			DrawSavedDetections(currentPreviewImgPath, jsonPath, targetClassId);
		} else {
			DrawAIBoundingBoxes(targetClassId);
		}
	}
}
private: System::Void btnCloseAI_Click(System::Object^ sender, System::EventArgs^ e) {
	pnlAIPreview->Visible = false;
}
private: System::Void btnDownloadSlip_Click(System::Object^ sender, System::EventArgs^ e) {
	// Pull relative location payload from list row tag memory
	System::String^ secretData = safe_cast<System::String^>(dgvHistory->Rows[dgvHistory->CurrentRow->Index]->Tag);
	System::String^ slipPath = secretData->Split('|')[0];

	if (System::IO::File::Exists(slipPath)) {
		System::Windows::Forms::SaveFileDialog^ saveFile = gcnew System::Windows::Forms::SaveFileDialog();
		saveFile->Filter = L"JPEG Image|*.jpg";
		saveFile->FileName = dgvHistory->Rows[dgvHistory->CurrentRow->Index]->Cells[0]->Value->ToString() + L"_Slip.jpg";

		if (saveFile->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			System::IO::File::Copy(slipPath, saveFile->FileName, true);
			MessageBox::Show(L"ดาวน์โหลดใบเสร็จเรียบร้อยแล้ว!", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
	}
}
	   // 1. ฟังก์ชันรับรูปมาโชว์ที่หน้าจอ
	private: void UpdateVideoFrame(System::Drawing::Image^ frame, System::Drawing::Image^ rawFrame, System::String^ data, System::String^ jsonBoxData) {
		if (picUploadAreaNew->Image != nullptr) delete picUploadAreaNew->Image;
		picUploadAreaNew->Image = frame;
		lblUploadTextNew->Visible = false;

		// 🌟 อัปเดตความจำให้เป็นตัวเลขล่าสุดเสมอ
		latestAiData = data;

		// ⚡ 4. ป้องกันบั๊กรูป Raw เป็น Null (ดักไว้ว่าถ้า Python ไม่ได้ส่งมา ให้เก็บรูปเดิมไว้ อย่าลบทิ้ง)
		if (rawFrame != nullptr) {
			if (currentRawVideoFrame != nullptr) delete currentRawVideoFrame;
			currentRawVideoFrame = rawFrame;
		}

		if (jsonBoxData != nullptr && jsonBoxData != L"") {
			currentBoxData = jsonBoxData;
		}

		System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(picUploadAreaNew->Image);
		System::Drawing::Font^ font = gcnew System::Drawing::Font(L"Tahoma", 14, System::Drawing::FontStyle::Bold);
		
		array<System::String^>^ parts = data->Split('|');
		if (parts->Length == 6) {
			float cursorX = 10.0f;
			float cursorY = 10.0f;

			System::String^ prefix = L"AI Data: [ ";
			g->DrawString(prefix, font, System::Drawing::Brushes::White, cursorX, cursorY);
			cursorX += g->MeasureString(prefix, font).Width - 8.0f;

			// เรียงลำดับสีตามข้อมูลจาก Python: Ripe, Underripe, Overripe, Unripe, Empty, Rotten
			array<System::Drawing::Brush^>^ colors = {
				System::Drawing::Brushes::LimeGreen,   // 0: Ripe
				System::Drawing::Brushes::Gold,        // 1: Underripe
				System::Drawing::Brushes::Orange,      // 2: Overripe
				System::Drawing::Brushes::DeepSkyBlue, // 3: Unripe
				System::Drawing::Brushes::LightCoral,  // 4: Empty
				System::Drawing::Brushes::Silver       // 5: Rotten
			};

			for (int i = 0; i < 6; i++) {
				g->DrawString(parts[i], font, colors[i], cursorX, cursorY);
				cursorX += g->MeasureString(parts[i], font).Width - 5.0f;
				
				if (i < 5) {
					g->DrawString(L" | ", font, System::Drawing::Brushes::White, cursorX, cursorY);
					cursorX += g->MeasureString(L" | ", font).Width - 5.0f;
				}
			}
			g->DrawString(L" ]", font, System::Drawing::Brushes::White, cursorX, cursorY);
		} else {
			g->DrawString(L"AI Data: " + data, font, System::Drawing::Brushes::Yellow, 10, 10);
		}

		delete font;
		delete g;

		picUploadAreaNew->Refresh();
	}


		   // Background Daemon Loop (Listens to Localhost Socket connected to Python Backend)
	private: void VideoReceiverLoop() {
		try {
			videoListener = gcnew TcpListener(System::Net::IPAddress::Parse("127.0.0.1"), 8888);
			videoListener->Start();

			while (isVideoRunning) {
				if (videoListener->Pending()) {
					TcpClient^ client = videoListener->AcceptTcpClient();
					NetworkStream^ stream = client->GetStream();
					BinaryReader^ reader = gcnew BinaryReader(stream);

					while (isVideoRunning && client->Connected) {
						try {
							int imgSize = reader->ReadInt32();
							if (imgSize <= 0) break;

							array<System::Byte>^ imgBytes = ReadExactBytes(stream, imgSize);
							MemoryStream^ ms = gcnew MemoryStream(imgBytes);
							System::Drawing::Image^ tempFrame = System::Drawing::Image::FromStream(ms);
							System::Drawing::Bitmap^ frame = gcnew System::Drawing::Bitmap(tempFrame);
							delete tempFrame;
							ms->Close();

							int rawSize = reader->ReadInt32();
							System::Drawing::Image^ rawFrame = nullptr;
							if (rawSize > 0) {
								array<System::Byte>^ rawBytes = ReadExactBytes(stream, rawSize);
								MemoryStream^ rawMs = gcnew MemoryStream(rawBytes);
								System::Drawing::Image^ tempRaw = System::Drawing::Image::FromStream(rawMs);
								rawFrame = gcnew System::Drawing::Bitmap(tempRaw);
								delete tempRaw;
								rawMs->Close();
							}

							int textSize = reader->ReadInt32();
							System::String^ aiData = L"";
							if (textSize > 0) {
								array<System::Byte>^ textBytes = ReadExactBytes(stream, textSize);
								aiData = System::Text::Encoding::UTF8->GetString(textBytes);
							}

							int jsonSize = reader->ReadInt32();
							System::String^ jsonBoxData = L"";
							if (jsonSize > 0) {
								array<System::Byte>^ jsonBytes = ReadExactBytes(stream, jsonSize);
								jsonBoxData = System::Text::Encoding::UTF8->GetString(jsonBytes);
							}

							this->Invoke(gcnew UpdateVideoFrameDelegate(this, &MainForm::UpdateVideoFrame), frame, rawFrame, aiData, jsonBoxData);

						}
						catch (System::IO::EndOfStreamException^) {
							break; // วิดีโอหยุดอย่างสันติ
						}
						catch (System::Exception^ ex) {
							// แจ้งเตือนกรณีท่อหลุดจริงๆ โดยไม่ได้กดปุ่มหยุด
							if (isVideoRunning) {
								MessageBox::Show(L"สัญญาณวิดีโอขาดหาย หรือเกิดข้อผิดพลาดในการดึงภาพ", L"Stream Disconnected", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							}
							break;
						}
					}
					reader->Close();
					stream->Close();
					client->Close();
				}
				Thread::Sleep(10);
			}
			videoListener->Stop();
		}
		catch (System::Exception^ ex) {
			MessageBox::Show(L"เกิดข้อผิดพลาดเบื้องหลังกับท่อ C++ (พอร์ตอาจจะถูก Process ผีสิงจับไว้):\n" + ex->Message, L"Socket Server Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (...) {}
	}
	// Action Listener for initialization logic
	private: System::Void btnStartVideo_Click(System::Object^ sender, System::EventArgs^ e) {
		if (cmbCameraSource->SelectedIndex == -1) {
			MessageBox::Show(L"กรุณาเลือกรูปแบบการทำงานในช่อง Dropdown ก่อนครับ", L"Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}

		int selectedMode = cmbCameraSource->SelectedIndex;

		// Mode 0: Local Image Read Loop
		if (selectedMode == 0) {
			System::Windows::Forms::OpenFileDialog^ openFileDialog = gcnew System::Windows::Forms::OpenFileDialog();
			openFileDialog->Filter = L"Image Files (*.jpg;*.jpeg;*.png)|*.jpg;*.jpeg;*.png";

			if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				try {
					if (picUploadAreaNew->Image != nullptr) delete picUploadAreaNew->Image;

					System::Drawing::Image^ rawImg = System::Drawing::Image::FromFile(openFileDialog->FileName);
					System::Drawing::Image^ aiReadyImg = CreateLetterboxImage(rawImg, 640, 640);
					delete rawImg;

					picUploadAreaNew->Image = aiReadyImg;
					picUploadAreaNew->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
					lblUploadTextNew->Visible = false;

					System::String^ docPath = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments);
					System::String^ imgFolder = docPath + L"\\FFB_History\\Images";
					if (!System::IO::Directory::Exists(imgFolder)) System::IO::Directory::CreateDirectory(imgFolder);

					currentOriginalImgPath = imgFolder + L"\\IMG_" + System::DateTime::Now.ToString(L"yyyyMMdd_HHmmss") + L".jpg";
					aiReadyImg->Save(currentOriginalImgPath, System::Drawing::Imaging::ImageFormat::Jpeg);

					// Purge overlapping visibility logic
					UpdateDisplayConfig();

					// Instruct deep-copy invocation logic using YOLO bridge
					this->Cursor = System::Windows::Forms::Cursors::WaitCursor;
					System::String^ tempImgPath = currentOriginalImgPath;
					std::string imgPath = msclr::interop::marshal_as<std::string>(tempImgPath);
					std::string modelPath = "C:\\projectsky\\Final300.onnx";
					cv::Mat resultImg;
					std::string boxDataStr = "";
					int tE = 0, tO = 0, tR = 0, tRot = 0, tU = 0, tUn = 0;

					bool success = RunYOLOv8_AI(imgPath, modelPath, resultImg, tE, tO, tR, tRot, tU, tUn, boxDataStr);
					this->Cursor = System::Windows::Forms::Cursors::Default;

					if (success && !resultImg.empty()) {
						currentBoxData = gcnew System::String(boxDataStr.c_str());
						if (picUploadAreaNew->Image != nullptr) delete picUploadAreaNew->Image;
						picUploadAreaNew->Image = MatToBitmap(resultImg);

						// Memorize layout for Analyze handler trigger
						latestAiData = tR.ToString() + L"|" + tU.ToString() + L"|" + tO.ToString() + L"|" + tUn.ToString() + L"|" + tE.ToString() + L"|" + tRot.ToString();

					}
					else {
						MessageBox::Show(L"วิเคราะห์รูปภาพไม่สำเร็จ", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					}
				}
				catch (...) {
					MessageBox::Show(L"ไม่สามารถโหลดไฟล์รูปภาพได้", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			return;
		}

		// Modes 1-3: Streaming configurations passing parameters to Python AI
		System::String^ configSource = L"0";

		if (selectedMode == 1) { // Media File
			System::Windows::Forms::OpenFileDialog^ openFileDialog = gcnew System::Windows::Forms::OpenFileDialog();
			openFileDialog->Filter = L"Video Files (*.mp4;*.avi)|*.mp4;*.avi";

			if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				currentOriginalImgPath = openFileDialog->FileName;
				configSource = currentOriginalImgPath;

				if (picUploadAreaNew->Image != nullptr) delete picUploadAreaNew->Image;
				picUploadAreaNew->Image = nullptr;
				lblUploadTextNew->Visible = true;
				lblUploadTextNew->Text = L"🎬 โหลดไฟล์วิดีโอแล้ว\nกำลังรอการเชื่อมต่อจาก AI...";
			}
			else {
				return;
			}
		}
		else if (selectedMode == 2) {
			configSource = L"1"; // Built-in Webcam
		}
		else if (selectedMode == 3) {
			System::Windows::Forms::Form^ prompt = gcnew System::Windows::Forms::Form();
			prompt->Width = 400; prompt->Height = 180;
			prompt->Text = L"ตั้งค่า IP กล้องมือถือ (Phone Camera)";
			prompt->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			prompt->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			prompt->MaximizeBox = false; prompt->MinimizeBox = false;

			System::Windows::Forms::Label^ lbl = gcnew System::Windows::Forms::Label();
			lbl->Left = 20; lbl->Top = 20; lbl->Width = 350;
			lbl->Text = L"ระบุ URL ของกล้องมือถือ (เช่น http://192.168.1.10:8080/video):";

			System::Windows::Forms::TextBox^ txtIp = gcnew System::Windows::Forms::TextBox();
			txtIp->Left = 20; txtIp->Top = 50; txtIp->Width = 340;

			// Check previous file contents for existing IP
			if (System::IO::File::Exists(L"C:\\projectsky\\camera_source.txt")) {
				System::String^ lastTxt = System::IO::File::ReadAllText(L"C:\\projectsky\\camera_source.txt")->Trim();
				if (lastTxt->StartsWith(L"http")) txtIp->Text = lastTxt;
				else txtIp->Text = L"http://172.30.90.202:8080/video";
			} else {
				txtIp->Text = L"http://172.30.90.202:8080/video";
			}

			System::Windows::Forms::Button^ btnOk = gcnew System::Windows::Forms::Button();
			btnOk->Text = L"ตกลง"; btnOk->Left = 250; btnOk->Top = 90; btnOk->Width = 110;
			btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;

			prompt->Controls->Add(lbl);
			prompt->Controls->Add(txtIp);
			prompt->Controls->Add(btnOk);
			prompt->AcceptButton = btnOk;

			if (prompt->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				configSource = txtIp->Text->Trim();
				if (System::String::IsNullOrEmpty(configSource)) {
					MessageBox::Show(L"กรุณาระบุ IP ก่อนทำงาน", L"Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}
			} else {
				return; // User cancelled
			}
		}

		// Save state target for asynchronous startup hook
		System::IO::File::WriteAllText(L"C:\\projectsky\\camera_source.txt", configSource);

		// Soft-lock GUI elements until teardown is requested
		btnStartVideo->Enabled = false;
		btnStopVideo->Enabled = true;
		cmbCameraSource->Enabled = false;

		// Attach Socket Loop
		isVideoRunning = true;
		videoThread = gcnew Thread(gcnew ThreadStart(this, &MainForm::VideoReceiverLoop));
		videoThread->IsBackground = true;
		videoThread->Start();

		// Spawn Subsystem Child Process with Virtual Environment
		try {
			System::Diagnostics::ProcessStartInfo^ psi = gcnew System::Diagnostics::ProcessStartInfo();

			// ใช้ Python จาก venv โดยตรง
			psi->FileName = L"C:\\projectsky\\venv\\Scripts\\python.exe";
			psi->Arguments = L"ai_server.py";
			psi->WorkingDirectory = L"C:\\projectsky";

			psi->UseShellExecute = false;
			psi->CreateNoWindow = true; 
			psi->WindowStyle = System::Diagnostics::ProcessWindowStyle::Hidden;

			pythonProcess = System::Diagnostics::Process::Start(psi);
		}
		catch (System::Exception^ ex) {
			MessageBox::Show(L"เกิดข้อผิดพลาดในการเปิด Python อัตโนมัติ: " + ex->Message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
	// Action Listener for teardown logic
	private: System::Void btnStopVideo_Click(System::Object^ sender, System::EventArgs^ e) {
		isVideoRunning = false;
		if (videoListener != nullptr) {
			videoListener->Stop();
		}

		// Re-secure and close out terminal handle logic properly
		if (pythonProcess != nullptr && !pythonProcess->HasExited) {
			pythonProcess->Kill();
			pythonProcess = nullptr;
		}

		btnStartVideo->Enabled = true;
		btnStopVideo->Enabled = false;
		cmbCameraSource->Enabled = true;
	}
private: System::Void btnApplyResult_Click(System::Object^ sender, System::EventArgs^ e) {
	if (System::String::IsNullOrEmpty(latestAiData)) {
		MessageBox::Show(L"ยังไม่มีข้อมูลผลลัพธ์จาก AI ครับ กรุณาเปิดกล้องสแกนก่อน", L"Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	try {
		// Read metrics pipeline layout
		array<System::String^>^ counts = latestAiData->Split('|');

		if (counts->Length >= 6) {
			// Serialize logic (Expected Formatting : Ripe | Underripe | Overripe | Unripe | Empty | Rotten)
			int cRipe = Convert::ToInt32(counts[0]);
			int cUnderripe = Convert::ToInt32(counts[1]);
			int cOverripe = Convert::ToInt32(counts[2]);
			int cUnripe = Convert::ToInt32(counts[3]);
			int cEmpty = Convert::ToInt32(counts[4]);
			int cRotten = Convert::ToInt32(counts[5]);

			MessageBox::Show(L"ดึงข้อมูลลงตารางสำเร็จเรียบร้อย!\nพร้อมคำนวณยอดเงินแล้วครับ", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
	}
	catch (...) {
		MessageBox::Show(L"เกิดข้อผิดพลาดในการดึงข้อมูล กรุณาลองใหม่", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}	
	// Graph Update and UI Render Logic Core Function Wrapper
	private: void ProcessAndCalculateData(int cEmpty, int cOverripe, int cRipe, int cRotten, int cUnderripe, int cUnripe) {
		// Assert Weight Requirements Defined
		try { curNetWeight = Convert::ToDouble(label11->Text); }
		catch (...) { curNetWeight = 0; }

		if (curNetWeight <= 0) {
			MessageBox::Show(L"กรุณากรอกน้ำหนักรถเข้า-ออก ให้ถูกต้องก่อนวิเคราะห์ครับ", L"แจ้งเตือน", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}

		// Re-initialize core local constants with new transaction definitions
		curEmpty = cEmpty;
		curOverripe = cOverripe;
		curRipe = cRipe;
		curRotten = cRotten;
		curUnderripe = cUnderripe;
		curUnripe = cUnripe;

		// Standard Graph Load Operations
		chartRipeness->Series[0]->Points->Clear();
		
		chartRipeness->Series[0]->Points->AddXY(L"สุก", curRipe); chartRipeness->Series[0]->Points[0]->Color = Color::LimeGreen; chartRipeness->Series[0]->Points[0]->LegendText = L"สุก: " + curRipe.ToString(); chartRipeness->Series[0]->Points[0]->Label = (curRipe > 0) ? L"สุก" : L" ";
		
		chartRipeness->Series[0]->Points->AddXY(L"ห่าม", curUnderripe); chartRipeness->Series[0]->Points[1]->Color = Color::Gold; chartRipeness->Series[0]->Points[1]->LegendText = L"ห่าม: " + curUnderripe.ToString(); chartRipeness->Series[0]->Points[1]->Label = (curUnderripe > 0) ? L"ห่าม" : L" ";
		
		chartRipeness->Series[0]->Points->AddXY(L"สุกเกิน", curOverripe); chartRipeness->Series[0]->Points[2]->Color = Color::Orange; chartRipeness->Series[0]->Points[2]->LegendText = L"สุกเกิน: " + curOverripe.ToString(); chartRipeness->Series[0]->Points[2]->Label = (curOverripe > 0) ? L"สุกเกิน" : L" ";
		
		chartRipeness->Series[0]->Points->AddXY(L"ดิบ", curUnripe); chartRipeness->Series[0]->Points[3]->Color = Color::DeepSkyBlue; chartRipeness->Series[0]->Points[3]->LegendText = L"ดิบ: " + curUnripe.ToString(); chartRipeness->Series[0]->Points[3]->Label = (curUnripe > 0) ? L"ดิบ" : L" ";
		
		chartRipeness->Series[0]->Points->AddXY(L"ทะลายเปล่า", curEmpty); chartRipeness->Series[0]->Points[4]->Color = Color::LightCoral; chartRipeness->Series[0]->Points[4]->LegendText = L"ทะลายเปล่า: " + curEmpty.ToString(); chartRipeness->Series[0]->Points[4]->Label = (curEmpty > 0) ? L"ทะลายเปล่า" : L" ";
		
		chartRipeness->Series[0]->Points->AddXY(L"เน่า", curRotten); chartRipeness->Series[0]->Points[5]->Color = Color::Silver; chartRipeness->Series[0]->Points[5]->LegendText = L"เน่า: " + curRotten.ToString(); chartRipeness->Series[0]->Points[5]->Label = (curRotten > 0) ? L"เน่า" : L" ";

		// Penalty Matrix Price Modifier Hooks
		int totalBunches = curEmpty + curOverripe + curRipe + curRotten + curUnderripe + curUnripe;
		curTotalDeduction = 0;
		if (totalBunches > 0) {
			curTotalDeduction += ((double)curEmpty / totalBunches) * dedEmpty;
			curTotalDeduction += ((double)curOverripe / totalBunches) * dedOverripe;
			curTotalDeduction += ((double)curRipe / totalBunches) * dedRipe;
			curTotalDeduction += ((double)curRotten / totalBunches) * dedRotten;
			curTotalDeduction += ((double)curUnderripe / totalBunches) * dedUnderripe;
			curTotalDeduction += ((double)curUnripe / totalBunches) * dedUnripe;
		}

		// Compute final pricing structure with discounts and load to UI
		curNetRate = currentBasePrice - curTotalDeduction;
		if (curNetRate < 0) curNetRate = 0;
		curTotalPrice = curNetWeight * curNetRate;

		lblBasePriceShow->Text = currentBasePrice.ToString(L"F2");
		lblQualityDeductShow->Text = L"-" + curTotalDeduction.ToString(L"F2");
		lblNetRateShow->Text = curNetRate.ToString(L"F2");
		lblTotalPrice->Text = curTotalPrice.ToString(L"N2");
	}
private: System::Void cmbCameraSource_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	if (cmbCameraSource->SelectedIndex == 0) {
		btnStartVideo->Text = L"🖼️ อัปโหลดรูปภาพ";
	}
	else if (cmbCameraSource->SelectedIndex == 1) {
		btnStartVideo->Text = L"🎬 อัปโหลดวิดีโอ";
	}
	else if (cmbCameraSource->SelectedIndex == 2) {
		btnStartVideo->Text = L"📷 เปิดกล้องโน้ตบุ๊ก";
	}
	else if (cmbCameraSource->SelectedIndex == 3) {
		btnStartVideo->Text = L"📱 เชื่อมกล้องมือถือ";
	}
}
private: System::Void chartAIPreview_Click(System::Object^ sender, System::EventArgs^ e) {
}

private: System::Void chkHistBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	UpdateDisplayConfig(); 
	
	int targetClassId = cmbAIFilter->SelectedIndex - 1;

	// Automatically attempt filter state persistence using JSON structural targets
	if (pnlAIPreview->Visible) {
		if (picAI->Tag != nullptr) {
			System::String^ jsonPath = safe_cast<System::String^>(picAI->Tag);
			DrawSavedDetections(currentPreviewImgPath, jsonPath, targetClassId);
		} else {
			DrawAIBoundingBoxes(targetClassId);
		}
	}
}
private: System::Void chkHistMask_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	UpdateDisplayConfig();

	int targetClassId = cmbAIFilter->SelectedIndex - 1;

	if (pnlAIPreview->Visible) {
		if (picAI->Tag != nullptr) {
			System::String^ jsonPath = safe_cast<System::String^>(picAI->Tag);
			DrawSavedDetections(currentPreviewImgPath, jsonPath, targetClassId);
		} else {
			DrawAIBoundingBoxes(targetClassId);
		}
	}
}
};
}
