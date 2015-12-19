#pragma once
#include <stdio.h>
#include <math.h>

namespace It_lab_graphic {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		FILE* fp;
		int n,w,h; // w - width, h-height
		double a,b,a1,b1;//a1,b1- границы значений по y
		double y11,y22; // значения фун-ции f 
		double *mas;
		double *f;
		int count; // если count=1, то выполнить отрисовку. Если count=0, отрисовку не выполнять
		Form1(void)
		{
			InitializeComponent();
			count=0;
			a1=0;
			b1=1;
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->Location = System::Drawing::Point(12, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(432, 324);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox1_Paint);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(466, 47);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(90, 31);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Graph";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(635, 364);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				
				fp=fopen("array_of_points.txt","r");


				fscanf(fp,"%d %lf %lf \n",&n,&a,&b); // cчитываем кол-во точек и границы отрезка
				fscanf(fp,"%lf %lf",&y22,&y11); // считывает границы фун-ции f
				mas=new double[n];
				f=new double[n];
				count=1;
				mas[0]=0;
				mas[n-1]=0;
				f[0]=0;
				for (int j=1;j<99;j++)
				{
					for(int i=1;i<n-1;i++)
					{
						fscanf(fp,"%lf",&mas[i]); // считываем точки
						if ((i*w/n>w*5/(b-a)) && (i*w/n<w*5.2/(b-a))) f[i]=2; // считаем f
						else f[i]=0;
					}	
					pictureBox1->Refresh();
				}
			//	delete [] mas;
			//	delete [] f;
				fclose(fp);
			 }
	
	private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				Pen^ pen = gcnew Pen(System::Drawing::Color::Black); 

				if (count!=0) 
				{
					int x1,x2,y1,y2; // дополнительные переменные для рисования отрезка
					for(int i=1;i<n-1;i++)
					{
						x1=Math::Round((i*w)/n);
						y1=Math::Round(h-mas[i]*h/(b1-a1));
						x2=Math::Round(((i+1)*w)/n);
						y2=Math::Round(h-mas[i+1]*h/(b1-a1));
						e->Graphics->DrawLine(pen,x1,y1,x2,y2);
						e->Graphics->DrawLine(pen,i*w/n,h-f[i-1]*h/(y22-y11),(i+1)*w/n,h-f[i]*h/(y22-y11));
					}
					fprintf(fp,"\n");
				}
			 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
 				w=pictureBox1->Width;
				h=pictureBox1->Height;

		 }
};
}

// password : Cn3gf5Zy2FX9