#pragma once
class TPoint
{
protected: 
	int x;
	int y;
public:
	TPoint(void);
	TPoint (int _x=0,int _y=0)
	{
		x=_x;
		y=_y;
	}
virtual void setR(int _Rx, int _Ry)	{}
virtual void Show (System::Drawing::Graphics^ g) 
	{
		System::Drawing::Pen^ pen=gcnew System::Drawing::Pen(System::Drawing::Color::Black);
		g->DrawEllipse(pen,x,y,1,1);
	}
virtual void Hide (System::Drawing::Graphics^ g)
	{
		System::Drawing::Pen^ pen=gcnew System::Drawing::Pen(System::Drawing::Color::White);
		g->DrawEllipse(pen,x,y,1,1);
	}
void Move (System::Drawing::Graphics^ g,int dx, int dy)  
	{
		Hide(g);
		x+=dx;
		y+=dy;
		Show(g);
	}
};

