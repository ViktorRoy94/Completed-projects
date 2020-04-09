#include "Figure.h"
#include "stdafx.h"

void getPictureBoxSize(int _w, int _h);
void definePosition(int x, int y, int &col, int &row);
void defineXY(int col, int row, int &x, int &y);
void startGame(System::Windows::Forms::PictureBox^ picture,System::Drawing::Graphics^ g);
void finishGame();
int checkClick(int x, int y,System::Windows::Forms::PictureBox^ picture,System::Drawing::Graphics^ g);