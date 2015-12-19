#pragma once
#include "stdafx.h"
namespace OperationSystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	protected:
		int numClusters; // кол-во кластеров. 
		double Probability; 
		int neverProgram; // кол-во невозможных программ
		int competePrograms; // кол-во выполненных программ
		int freeClusters; // кол-во свободных кластеров
		int count;
		int queueProgram; // кол-во программ в очереди
		queue queuePrograms;
		double taskLoad;
		int tickCPU;
		int createdPrograms;
		static int CountTakt = 0;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::Label^  label8;
	protected: 
	private: System::Windows::Forms::TextBox^  textBox4;

	protected: 



			 
	public:
		Form1(void)
		{
			InitializeComponent();

			
			competePrograms=0;
			neverProgram=0;
			taskLoad=0;
			queueProgram=0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	protected: 
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  Label2;
	private: System::Windows::Forms::Label^  Label1;




	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->Label2 = (gcnew System::Windows::Forms::Label());
			this->Label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(0, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(400, 302);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox1_refresh);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(598, 41);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(29, 20);
			this->textBox1->TabIndex = 1;
			this->textBox1->Text = L"0,5";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(598, 15);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(29, 20);
			this->textBox2->TabIndex = 2;
			this->textBox2->Text = L"12";
			// 
			// Label2
			// 
			this->Label2->AutoSize = true;
			this->Label2->Location = System::Drawing::Point(528, 44);
			this->Label2->Name = L"Label2";
			this->Label2->Size = System::Drawing::Size(55, 13);
			this->Label2->TabIndex = 3;
			this->Label2->Text = L"Probability";
			// 
			// Label1
			// 
			this->Label1->AutoSize = true;
			this->Label1->Location = System::Drawing::Point(524, 18);
			this->Label1->Name = L"Label1";
			this->Label1->Size = System::Drawing::Size(64, 13);
			this->Label1->TabIndex = 4;
			this->Label1->Text = L"numClusters";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(552, 76);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Start";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(552, 105);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Stop";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(591, 135);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(36, 20);
			this->textBox3->TabIndex = 7;
			this->textBox3->Text = L"0";
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(591, 161);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(36, 20);
			this->textBox4->TabIndex = 8;
			this->textBox4->Text = L"0";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(484, 138);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(97, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Programs in Queue";
			this->label3->Click += gcnew System::EventHandler(this, &Form1::label3_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(484, 164);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(101, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Complete Programs ";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(484, 190);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(83, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Never Programs";
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(591, 187);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(36, 20);
			this->textBox5->TabIndex = 12;
			this->textBox5->Text = L"0";
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(591, 213);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(36, 20);
			this->textBox6->TabIndex = 13;
			this->textBox6->Text = L"0";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(484, 216);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 13);
			this->label6->TabIndex = 14;
			this->label6->Text = L"Tick CPU";
			this->label6->Click += gcnew System::EventHandler(this, &Form1::label6_Click);
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(591, 240);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(36, 20);
			this->textBox7->TabIndex = 15;
			this->textBox7->Text = L"0";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(484, 243);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(91, 13);
			this->label7->TabIndex = 16;
			this->label7->Text = L"Created Programs";
			// 
			// textBox8
			// 
			this->textBox8->Location = System::Drawing::Point(591, 266);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(36, 20);
			this->textBox8->TabIndex = 17;
			this->textBox8->Text = L"0";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(484, 269);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(58, 13);
			this->label8->TabIndex = 18;
			this->label8->Text = L"Task Load";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(650, 317);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->textBox8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->textBox6);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->Label1);
			this->Controls->Add(this->Label2);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		    timer1->Start();
			numClusters=Convert::ToInt32(textBox2->Text); // считываем кол-во Кластеров
			freeClusters=numClusters;
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 timer1->Stop();
			 textBox8->Text=((double)taskLoad/count).ToString();
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {

			 CountTakt++;
			
			 numClusters=Convert::ToInt32(textBox2->Text); // считываем кол-во Кластеров
			 Probability=Convert::ToDouble(textBox1->Text); // считываем вероятность
			 for(int i=0;i<3;i++)
			 {
				srand(time(NULL));
				int q = rand() % 100; // берем случайное число от 0 до 100 
				double beta = (double) q / 100; // beta - вероятноясть появления программы (от 0.00 до 1.00) 
				if (beta>Probability) // если beta больше исходной вероятности, то создаем программу.
					{
					 program a(numClusters);
					 createdPrograms++;
					 if (a.GetNumCluster()<numClusters+1)
					 {
						 a.SetStatus(0);
						 queuePrograms.Enqueue(a);
						 queueProgram++;
					 }
					 else
					 {
						 neverProgram++;
					 }
					}
			 }
			 program* w = new program[100];
			 int m=queuePrograms.GetIndexEnd();
			 int n=queuePrograms.GetIndexBegin();
			 if (n>m) 
			 {
				 n-=1000;
			 }

			 for (int i=0;i<abs(m-n); i++)
			 {
				int k=0; // 1 - не нужно возвращать в очередь. 0 - нужно
				w[i]= queuePrograms.Dequeue(); // вынутая из очереди программа
				queueProgram--;
				int time = w[i].GetTime();// время нужное на выполнение программы из очереди
				
				if((time==1) && (w[i].GetStatus()==1))
				 {
					 competePrograms++;
					 freeClusters+=w[i].GetNumCluster();
					 k++;
				}

				if ((time>1) && (w[i].GetStatus()==1))
				{
					 w[i].SetTime(time-1);
				}
				
				if ((time>1) && (w[i].GetStatus()!=1) && (w[i].GetNumCluster()<freeClusters+1))
				{
					freeClusters-=w[i].GetNumCluster();
					w[i].SetTime(time-1);
					w[i].SetStatus(1);		
				}
				
				if (k==0) // проверка на выполненность
				{
					queuePrograms.Enqueue(w[i]);
					queueProgram++;
				}
				count++;
				taskLoad+=((double)(numClusters-freeClusters)/numClusters)*100;
				textBox3->Text=queueProgram.ToString();
				textBox4->Text=competePrograms.ToString();
				textBox5->Text=neverProgram.ToString();
				textBox6->Text=(CountTakt).ToString();
				textBox7->Text=createdPrograms.ToString();
				pictureBox1->Refresh();
			 }

		 }
private: System::Void pictureBox1_refresh(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 if(count!=0) 
			 {
				int b=10; // расстояние между кластерами
				int sizeRec=25; // сторона квадрата
				int x1=b,y1=b;
				int a=0; // счетчик пройденных занятых
				SolidBrush^ Brush = gcnew SolidBrush(System::Drawing::Color::Red); 
				for (int i=0;i<7;i++)
				{
					 for (int j=0;j<=numClusters/7+1;j++)
					{
						if (a==numClusters) break;
						if (a++>=numClusters-freeClusters) Brush->Color=Color::Green;
						x1=(sizeRec+b)*j;
						y1=(sizeRec+b)*i;
						e->Graphics->FillRectangle(Brush,x1,y1,sizeRec,sizeRec);

					}
				}	
			 }
			 
		 }
private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

