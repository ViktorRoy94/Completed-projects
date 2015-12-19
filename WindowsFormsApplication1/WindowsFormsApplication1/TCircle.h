#pragma once
#include "TPoint.h"
class TCircle:public TPoint
{
protected:
	int Rx,Ry;
public:
	TCircle(void);
	TCircle(int _x,int _y,int _Rx,int _Ry):TPoint(_x,_y)
	{}
	virtual void setR(int _Rx, int _Ry) override
	{
		Rx=_Rx;
		Ry=_Ry;
	}
	virtual void Show(System::Drawing::Graphics^ g) override
	{
		System::Drawing::Brush^ brush= gcnew System::Drawing::SolidBrush(System::Drawing::Color::Black);
		g->System::Drawing::Graphics::FillEllipse(brush,x,y,Rx,Ry); 
	}
	virtual void Hide(System::Drawing::Graphics^ g) override
	{
		System::Drawing::Brush^ brush= gcnew System::Drawing::SolidBrush(System::Drawing::Color::White);
		g->System::Drawing::Graphics::FillEllipse(brush,x,y,Rx,Ry);
	}

	~TCircle(void);
};

