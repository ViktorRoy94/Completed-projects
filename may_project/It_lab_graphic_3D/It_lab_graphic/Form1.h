#pragma once
#include <stdio.h>
#include <math.h>
#include <string>


namespace It_lab_graphic {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Runtime::InteropServices;
	using namespace std;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		FILE* fp_x;
		FILE* fp_y;
		FILE* fp_z;
		FILE* fp_IDSx;
		FILE* fp_IDSy;
		FILE* fp_IDSz;
		int n,w1,h1,w2,h2,w3,h3,nIDS; // w - width, h-height
		double a,b,a1,b1,aIDS,bIDS,a1IDS,b1IDS;//a1,b1- границы значений по y
		double y11,y22,yIDS11,yIDS22; // значения фун-ции f 
		double *mas,*mas1;
		double *f;
		int count; // если count=1, то выполнить отрисовку. Если count=0, отрисовку не выполнять
		
		void MarshalString ( String ^ s, string& os )  // ф-ция для перевода из String^ в  std::string
		{
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::PictureBox^  pictureBox3;


	public: 
		Form1(void)
		{
			InitializeComponent();
			
			count=0;
			a1=0;
			b1=3;
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
	private: System::Windows::Forms::Button^  button1;


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
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->Location = System::Drawing::Point(30, 9);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(431, 202);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox1_Paint);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(469, 218);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(100, 38);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Graph";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(14, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"X";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 231);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(14, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Y";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(906, 9);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(14, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Z";
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackColor = System::Drawing::Color::White;
			this->pictureBox2->Location = System::Drawing::Point(30, 217);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(431, 202);
			this->pictureBox2->TabIndex = 7;
			this->pictureBox2->TabStop = false;
			this->pictureBox2->Click += gcnew System::EventHandler(this, &Form1::pictureBox2_Click);
			this->pictureBox2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox2_Paint);
			// 
			// pictureBox3
			// 
			this->pictureBox3->BackColor = System::Drawing::Color::White;
			this->pictureBox3->Location = System::Drawing::Point(469, 9);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(431, 202);
			this->pictureBox3->TabIndex = 8;
			this->pictureBox3->TabStop = false;
			this->pictureBox3->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox3_Paint);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(926, 430);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				fp_x=fopen("filex.txt","r");
				fp_y=fopen("filey.txt","r"); 
				fp_z=fopen("filez.txt","r"); 
				fp_IDSx=fopen("filexIDS.txt","r");
				fp_IDSy=fopen("fileyIDS.txt","r"); 
				fp_IDSz=fopen("filezIDS.txt","r"); 

				fscanf(fp_z,"%d %lf %lf \n",&n,&a,&b); // cчитываем кол-во точек и границы отрезка
				fscanf(fp_z,"%lf %lf",&y22,&y11); // считывает границы фун-ции f
				fscanf(fp_IDSz,"%d %lf %lf \n",&nIDS,&aIDS,&bIDS); // cчитываем кол-во точек и границы отрезка
				fscanf(fp_IDSz,"%lf %lf",&yIDS22,&yIDS11); // считывает границы фун-ции f

				mas=new double[n*n*n];
				mas1=new double[(n-2)*(n-2)*(n-2)];
				f=new double[n*n*n];
				
				count=1;
				memset(mas, 0, n*n*n*sizeof(double));
				memset(mas1, 0, (n-2)*(n-2)*(n-2)*sizeof(double));
				memset(f, 0, n*n*n*sizeof(double));

//				for (int j=1;j<99;j++)
//				{
					//for(int i=1;i<n-1;i++)
					//{
					//	fscanf(fp_x,"%lf",&mas[i]); // считываем точки
					//	if ((i*w1/n>w1*5/(b-a)) && (i*w1/n<w1*5.2/(b-a))) f[i]=4; // считаем f
					//	else f[i]=0;
					//}
					//_sleep(100);
					
					
					//for(int i=1;i<n-1;i++)
					//{
					//	fscanf(fp_y,"%lf",&mas[i]); // считываем точки
					//	if ((i*w2/n>w2*5/(b-a)) && (i*w2/n<w2*5.2/(b-a))) f[i]=4; // считаем f
					//	else f[i]=0;
					//}
					//_sleep(100);
					
					
					for(int i=1;i<n-1;i++)
					{
						fscanf(fp_z,"%lf",&mas[i]); // считываем точки
						if (((i*w3)/n > (w3*5)/(b-a)) && ((i*w3)/n < (w3*5.2)/(b-a))) f[i]=4; // считаем f
						else f[i]=0;
					}
					for(int i=1;i<n-1;i++)
					{
						fscanf(fp_IDSz,"%lf",&mas1[i]); // считываем точки
						mas1[i]+=0.1;
					}

					_sleep(10);
					pictureBox3->Refresh();
					pictureBox2->Refresh();
					pictureBox1->Refresh();
//				}
				delete [] mas;
				delete [] mas1;
				delete [] f;
				fclose(fp_x);
				fclose(fp_y);
				fclose(fp_z);
				fclose(fp_IDSx);
				fclose(fp_IDSy);
				fclose(fp_IDSz);
			 }
	
	private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				Pen^ pen = gcnew Pen(System::Drawing::Color::Black); 

				if (count!=0) 
				{
					int x1,x2,y1,y2; // дополнительные переменные для рисования отрезка
					int xIDS1, yIDS1, zIDS1,xIDS2, yIDS2, zIDS2;
					for(int i=2;i<nIDS-1;i++)
					{
						x1=Math::Round((i*w1)/n);
						y1=Math::Round(h1-((mas[i]/(b1-a1))*h1)/2);
						x2=Math::Round(((i+1)*w1)/n);
						y2=Math::Round(h1-((mas[i+1]/(b1-a1))*h1)/2);

						xIDS1=Math::Round((i*w1)/nIDS);
						yIDS1=Math::Round(h1-((mas1[i]/(b1-a1))*h1)/2);
						xIDS2=Math::Round(((i+1)*w1)/nIDS);
						yIDS2=Math::Round(h1-((mas1[i+1]/(b1-a1))*h1)/2);
					
						pen->Color = Color::Black;
						e->Graphics->DrawLine(pen,x1,y1,x2,y2);
							pen->Color = Color::Red;
						e->Graphics->DrawLine(pen,xIDS1,yIDS1,xIDS2,yIDS2);

		//				e->Graphics->DrawLine(pen,i*w1/n,h1-f[i-1]*h1/(y22-y11)/2,(i+1)*w1/n,h1-f[i]*h1/(y22-y11)/2);
					}
				}
			 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
 				w1=pictureBox1->Width;
				h1=pictureBox1->Height;
				w2=pictureBox2->Width;
				h2=pictureBox2->Height;
				w3=pictureBox3->Width;
				h3=pictureBox3->Height;
		 }
private: System::Void pictureBox2_Click(System::Object^  sender, System::EventArgs^  e) {
		 
		 }
private: System::Void pictureBox2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				Pen^ pen = gcnew Pen(System::Drawing::Color::Black); 

				if (count!=0) 
				{
					int x1,x2,y1,y2; // дополнительные переменные для рисования отрезка
					int xIDS1, yIDS1, zIDS1,xIDS2, yIDS2, zIDS2;

					for(int i=1;i<n-1;i++)
					{
						x1=Math::Round((i*w1)/n);
						y1=Math::Round(h1-((mas[i]/(b1-a1))*h1)/2);
						x2=Math::Round(((i+1)*w1)/n);
						y2=Math::Round(h1-((mas[i+1]/(b1-a1))*h1)/2);

						xIDS1=Math::Round((i*w1)/nIDS);
						yIDS1=Math::Round(h1-((mas1[i]/(b1-a1))*h1)/2);
						xIDS2=Math::Round(((i+1)*w1)/nIDS);
						yIDS2=Math::Round(h1-((mas1[i+1]/(b1-a1))*h1)/2);
					
						pen->Color = Color::Black;
						e->Graphics->DrawLine(pen,x1,y1,x2,y2);
							pen->Color = Color::Red;
						e->Graphics->DrawLine(pen,xIDS1,yIDS1,xIDS2,yIDS2);
					}
				}

		 }
private: System::Void pictureBox3_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				Pen^ pen = gcnew Pen(System::Drawing::Color::Black); 

				if (count!=0) 
				{
					int x1,x2,y1,y2; // дополнительные переменные для рисования отрезка
					int xIDS1, yIDS1, zIDS1,xIDS2, yIDS2, zIDS2;
					for(int i=1;i<n-1;i++)
					{
						x1=Math::Round((i*w1)/n);
						y1=Math::Round(h1-((mas[i]/(b1-a1))*h1)/2);
						x2=Math::Round(((i+1)*w1)/n);
						y2=Math::Round(h1-((mas[i+1]/(b1-a1))*h1)/2);

						xIDS1=Math::Round((i*w1)/nIDS);
						yIDS1=Math::Round(h1-((mas1[i]/(b1-a1))*h1)/2);
						xIDS2=Math::Round(((i+1)*w1)/nIDS);
						yIDS2=Math::Round(h1-((mas1[i+1]/(b1-a1))*h1)/2);
					
						pen->Color = Color::Black;
						e->Graphics->DrawLine(pen,x1,y1,x2,y2);
							pen->Color = Color::Red;
						e->Graphics->DrawLine(pen,xIDS1,yIDS1,xIDS2,yIDS2);
					}
				}

		 }
};
}

// password : Cn3gf5Zy2FX9