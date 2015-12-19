#include <stdlib.h>
#include "Compare.h"
#include "crsMatrix.h"
#include "portable_time.h"
#include "printTestMatrix.h"
#include "readTestMatrix.h"
#include <iostream>
using namespace std;

int main (int argc, char *argv[])
{
	int n  = 100000;				// n*n размер матрицы
	int nzInString = 100;		    // к-во ненулевых элементов в строке
	
	if (argc > 1) 
	{
		n = atoi(argv[1]);
		nzInString = atoi(argv[2]);
	}
	
	int nz = nzInString * n;	// к-во ненулевых элементов

	double *value; // массив значений 
	int *col;	   // массив координат столбцов
	int *rowIndex; // массив индексов строк
	
	value = new double[nz];
	col = new int[nz];
	rowIndex = new int[n+1];

	setRandomMatrix1(value, col, rowIndex, n, nzInString);

	FILE *outputMatrix;			// для вывода crs матрицы
	FILE *outputVector;			// для вывода решения

	fopen_s(&outputMatrix, "outputMatrix.txt", "w");   
	fopen_s(&outputVector, "outputVector.txt", "w");   

	//printMatrix(value, col, rowIndex, n);
	//printMatrixToFile(value, col, rowIndex, n, outputMatrix);
	
	double *x = new double[n];          // вектор
	double *b = new double[n];			// результат умножения матрицы на вектор

	// заполняем вектор
	for (int i = 0; i < n; i++)
	{
		x[i] = rand() % 10 + 1;
	}	
	
	//printTestMatrix(rowIndex, col, value, x, n, nz);

	//readTestMatrix(rowIndex, col, value, x, n, nz);

	double time = PortableGetTime();

	// умножение crs матрицы на вектор
	Multiplicate(value, col, rowIndex, x, b, n);

	time = PortableGetTime() - time; 

	//printVectorToFile(b, n, outputVector);

	// проверка умножения
	//checkedSolution(value, col, rowIndex, x, n, nz);
	
	printf("\ntime = %lf", time);

	//char c = 0;
	//scanf("%d", c);
	
	fclose(outputMatrix);
	fclose(outputVector);
	delete [] value;
	delete [] col;
	delete [] rowIndex;
	delete [] x;
	delete [] b;
	return 0;
}

