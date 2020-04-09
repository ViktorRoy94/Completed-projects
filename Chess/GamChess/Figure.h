#pragma once
#include "Main.h"
#include "Form1.h"
#include <math.h>
//using namespace GamChess;
class Figure
{
private:
	int click; // �������� �� ������� �� ������ 
	int type; // 1-king, 2-queen, 3-rook, 4-knight, 5-bishop, 6-pawn
	int color; // 0 - white, 1 - black
	int col;
	int row;
	
public:
	Figure(void)
	{
		type=0;
		color=0;
		row=0;
		col=0;
		click=0;
	}

	Figure(int _type, int _color, int _col, int _row) // ��������� ��� ������, �� ���� � ���������� �� �����
	{
		type=_type;
		color=_color;
		row=_row;
		col=_col;
		click=0;
	}

	int getTypeFigure() // ���������� ��� ������
	{
		return type;
	}
	int getRow()
	{
		return row;
	}
	void setRow(int _row)
	{
		row=_row;
	}
	void setCol(int _col)
	{
		col=_col;
	}

	int getCol()
	{
		return col;
	}
	int getColor()
	{
		return color;
	}



	void drawFigure(System::Drawing::Graphics^ g) // ���������� ������
	{
		int x,y;
		defineXY(col,row,x,y);
		
		System::Drawing::Brush^ brush;
		if (color==1) brush= gcnew System::Drawing::SolidBrush(System::Drawing::Color::Green);
		else brush= gcnew System::Drawing::SolidBrush(System::Drawing::Color::Blue);
		
		g->System::Drawing::Graphics::FillEllipse(brush,x,y,20,20); 
	}
	
	void moveFigure(int _col, int _row,System::Drawing::Graphics^ g,System::Windows::Forms::PictureBox^ picture)
	{
		picture->Refresh();
		col=_col;
		row=_row;
	}

	void setClick(int _click)
	{
		click=_click;
	}

	int getClick()
	{
		return click;
	}

	bool checkMove(int col2, int row2) //�ol1,row1 ��������� ���������, col2,row2 -�������� ���������
	{
		int flag=0;
		switch(type)
		{
		case 1:  //  ������
			if (abs(row-row2)<2 && abs(col-col2)<2) return true;
			break;
		case 2:  //  �����
			if (((col==col2) || (row==row2)) || ((abs(col-col2)==abs(row-row2)))) return true;
			break;
		case 3:  //  �����
			if ((col=col2) || (row=row2)) return true; 
			break;
		case 4:  //  ����
			if (((abs(col-col2)==1) && (abs(row2-row)==2)) || ((abs(col-col2)==2) && (abs(row-row2)==1))) return true;
			break;
		case 5:  //  ����
			if ((abs(col-col2)==abs(row-row2))) return true;
			break;
		case 6:  //  �����
			if (((col==col2) && (row2-row==1) && (color==0)) || ((col==col2) && (row-row2==1) && (color==1))) return true;
			break;
		return false;
		}
	}
	bool checkFight(int col2, int row2) //�ol,row ��������� ���������, col2,row2 -�������� ���������
	{
		int flag=0;
		switch(type)
		{
		case 1:  //  ������
			if (abs(row-row2)<2 && abs(col-col2)<2) return true;
			break;
		case 2:  //  �����
			if (((col==col2) || (row==row2)) || ((abs(col-col2)==abs(row-row2)))) return true;
			break;
		case 3:  //  �����
			if ((col=col2) || (row=row2)) return true; 
			break;
		case 4:  //  ����
			if (((abs(col-col2)==1) && (abs(row2-row)==2)) || ((abs(col-col2)==2) && (abs(row-row2)==1))) return true;
			break;
		case 5:  //  ����
			if ((abs(col-col2)==abs(row-row2))) return true;
			break;
		case 6:  //  �����
			if ((abs(col-col2)==1 && (row2-row==1) && (color==0)) || (abs(col-col2)==1 && (row-row2==1) && (color==1))) return true;
			break;
		return false;
		}
	}



	~Figure()
	{
	}

};

