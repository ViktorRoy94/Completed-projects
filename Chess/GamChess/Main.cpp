#include"stdafx.h"
#include "Main.h"

int w,h;
Figure* mas[32];
int COLOR=0; // Цвет игрока, который ходит
void getPictureBoxSize(int _w, int _h)
{
	w=_w;
	h=_h;
}
void definePosition(int x, int y, int &col, int &row)
{
	col=(int)(x*8/w)+1;
	row=(int)((h-y)*8/h)+1;
}

void defineXY(int col, int row, int &x, int &y)
{
	x=(int)(w/8)*(col-1)+(int)(w/32);
	y=(int)(h/8)*(8-row)+(int)(h/32);
}

void startGame(System::Windows::Forms::PictureBox^ picture,System::Drawing::Graphics^ g)
{
	COLOR=0;
	for(int i=0;i<8;i++)
	{
		mas[i]=new Figure(6,0,i+1,2);
	}
	mas[8]=new Figure(5,0,3,1);
	mas[9]=new Figure(5,0,6,1);		
	mas[10]=new Figure(3,0,1,1);
	mas[11]=new Figure(3,0,8,1);
	mas[12]=new Figure(4,0,2,1);
	mas[13]=new Figure(4,0,7,1);
	mas[14]=new Figure(1,0,4,1);
	mas[15]=new Figure(2,0,5,1);

	for(int i=16;i<24;i++)
	{
		mas[i]=new Figure(6,1,i-15,7);
	}
	mas[24]=new Figure(5,1,3,8);
	mas[25]=new Figure(5,1,6,8);
	mas[26]=new Figure(3,1,1,8);
	mas[27]=new Figure(3,1,8,8);
	mas[28]=new Figure(4,1,2,8);
	mas[29]=new Figure(4,1,7,8);
	mas[30]=new Figure(1,1,4,8);
	mas[31]=new Figure(2,1,5,8);
	
	for (int i=0;i<32;i++)
	{
		mas[i]->drawFigure(g);
	}

}

int checkClick(int x, int y,System::Windows::Forms::PictureBox^ picture,System::Drawing::Graphics^ g)
{ 
	int col,row;
	int ourFigure=33; // фигура на которую кликнули сейчас
	int previousFigure=34; // фигура, на которую уже кликнули
	definePosition(x,y,col,row);

	
	for(int i=0;i<32;i++)
	{
		if (mas[i]->getClick()==1)
		{
			previousFigure=i;
		}

	}

	for(int i=0;i<32;i++)
	{
		if(col==mas[i]->getCol() && row==mas[i]->getRow())
		{
			ourFigure=i;
			mas[ourFigure]->setClick(1);
		}

	}
	if((ourFigure==33) && previousFigure!=34) // указали на пустое место. 
	{
		if (mas[previousFigure]->checkMove(col,row) && COLOR==mas[previousFigure]->getColor())
		{
			mas[previousFigure]->moveFigure(col,row,g,picture);
		
			for(int i=0;i<32;i++)
				mas[i]->drawFigure(g);
			mas[previousFigure]->setClick(0);
			if (COLOR==0) COLOR=1;
			else COLOR=0;
		}
	}
	else 
	{
		if ((mas[ourFigure]->getColor()==mas[previousFigure]->getColor() && previousFigure!=34)) 
		{
			mas[previousFigure]->setClick(0);
			mas[ourFigure]->setClick(1);
			
		}
		else
		{
			if (mas[previousFigure]->checkFight(col,row) && previousFigure!=34 && COLOR==mas[previousFigure]->getColor())
			{
				mas[ourFigure]->setCol(0);
				mas[ourFigure]->setRow(0);
				mas[ourFigure]->setClick(0);
				mas[previousFigure]->moveFigure(col,row,g,picture);
				for(int i=0;i<32;i++)
					mas[i]->drawFigure(g);
				mas[previousFigure]->setClick(0);
				if (COLOR==0) COLOR=1;
				else COLOR=0;
			}

		}

	}
	if (COLOR==0) return 0;
	else return 1;
}


void finishGame()
{

}


