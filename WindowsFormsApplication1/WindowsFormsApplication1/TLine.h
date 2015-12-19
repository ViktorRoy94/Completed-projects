#pragma once
#include "TPoint.h"
class TLine:public TPoint
{
private:
	double angle;
	int length;
	float dx,dy;
	
public:
	TLine(void);
	TLine(int _x,int _y, int _angle, int _length):TPoint(_x,_y)
	{
		angle=_angle;
		length=_length;
	}
	virtual void Show(System::Drawing::Graphics^ g) override
	{

		System::Drawing::Pen^ pen= gcnew System::Drawing::Pen(System::Drawing::Color::Black);

		dx=(cos(angle*3.14/180.0)*length)/2;
		dy=(sin(angle*3.14/180.0)*length)/2;
		g->System::Drawing::Graphics::DrawLine(pen,x-dx,y+dy,x+dx,y-dy); 
	}
	virtual void Hide(System::Drawing::Graphics^ g) override
	{
		System::Drawing::Pen^ pen= gcnew System::Drawing::Pen(System::Drawing::Color::White);
		g->System::Drawing::Graphics::DrawLine(pen,x-dx,y+dy,x+dx,y-dy); 
	}
};

