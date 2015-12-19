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
	int n  = 100000;				// n*n ������ �������
	int nzInString = 100;		    // �-�� ��������� ��������� � ������
	
	if (argc > 1) 
	{
		n = atoi(argv[1]);
		nzInString = atoi(argv[2]);
	}
	
	int nz = nzInString * n;	// �-�� ��������� ���������

	double *value; // ������ �������� 
	int *col;	   // ������ ��������� ��������
	int *rowIndex; // ������ �������� �����
	
	value = new double[nz];
	col = new int[nz];
	rowIndex = new int[n+1];

	setRandomMatrix1(value, col, rowIndex, n, nzInString);

	FILE *outputMatrix;			// ��� ������ crs �������
	FILE *outputVector;			// ��� ������ �������

	fopen_s(&outputMatrix, "outputMatrix.txt", "w");   
	fopen_s(&outputVector, "outputVector.txt", "w");   

	//printMatrix(value, col, rowIndex, n);
	//printMatrixToFile(value, col, rowIndex, n, outputMatrix);
	
	double *x = new double[n];          // ������
	double *b = new double[n];			// ��������� ��������� ������� �� ������

	// ��������� ������
	for (int i = 0; i < n; i++)
	{
		x[i] = rand() % 10 + 1;
	}	
	
	//printTestMatrix(rowIndex, col, value, x, n, nz);

	//readTestMatrix(rowIndex, col, value, x, n, nz);

	double time = PortableGetTime();

	// ��������� crs ������� �� ������
	Multiplicate(value, col, rowIndex, x, b, n);

	time = PortableGetTime() - time; 

	//printVectorToFile(b, n, outputVector);

	// �������� ���������
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

