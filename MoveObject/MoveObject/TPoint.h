#pragma once
class TPoint
{
protected: 
	int x;
	int y;
public:
	TPoint(void)
	{
	}
	TPoint (int _x=0,int _y=0)
	{
		x=_x;
		y=_y;
	}

void Show (System::Drawing::Graphics^ g) 
	{
		System::Drawing::SolidBrush^ pen=gcnew System::Drawing::SolidBrush(System::Drawing::Color::Blue);
		g->FillEllipse(pen,x,y,50,50);
	}
void Hide (System::Drawing::Graphics^ g)
	{
		System::Drawing::SolidBrush^ pen=gcnew System::Drawing::SolidBrush(System::Drawing::Color::White);
		g->FillEllipse(pen,x,y,50,50);
	}
void Move (System::Drawing::Graphics^ g,int dx, int dy)  
	{
		Hide(g);
		x=dx;
		y=dy;
		Show(g);
	}
};

