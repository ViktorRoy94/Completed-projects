#include <stdlib.h>
#include "setRandomMatrix.h"
#include "portable_time.h"
#include <omp.h>
#include <limits.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	//int processRank;
	//MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
	
	int n = 100000;				// n*n ������ �������
	int nzInString = 100;		    // �-�� ��������� ��������� � ������

	if (argc > 1)
	{
		n = atoi(argv[1]);
		nzInString = atoi(argv[2]);
	}

	int nz = nzInString * n;	// �-�� ��������� ���������

	double *value; // ������ �������� 
	int *colIndex;	   // ������ ��������� ��������
	int *row; // ������ �������� �����

	//nz = 5;

	value = new double[nz];
	row = new int[nz]; 
	colIndex = new int[n + 1];
	
	/*value[0] = 1.0;
	value[1] = 7.0;
	value[2] = 2.0;
	value[3] = 3.0;
	value[4] = 4.0;

	row[0] = 0.0;
	row[1] = 1.0;
	row[2] = 1.0;
	row[3] = 0.0;
	row[4] = 2.0;

	colIndex[0] = 0.0;
	colIndex[1] = 2.0;
	colIndex[2] = 3.0;
	colIndex[3] = 5.0;
*/

	setRandomMatrix1(value, row, colIndex, n, nzInString);

	FILE *outputMatrix;			// ��� ������ crs �������
	FILE *outputVector;			// ��� ������ �������

	fopen_s(&outputMatrix, "outputMatrix.txt", "w");
	fopen_s(&outputVector, "outputVector.txt", "w");

	//printMatrixToFile(value, colIndex, row, n, outputMatrix);

	double *x = new double[n];          // ������
	double *b = new double[n];			// ��������� ��������� ������� �� ������

	// ��������� ������
	for (int i = 0; i < n; i++)
	{
		x[i] = rand() % 10 + 1;
		b[i] = 0;
	}

	/*x[0] = 1.0;
	x[1] = 2.0;
	x[2] = 3.0;*/

	double time = MPI_Wtime();
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		for (int j = colIndex[i]; j < colIndex[i + 1]; j++) {
			b[row[j]] += value[j] * x[i];
		}
	}
	
	time = MPI_Wtime() - time;
	
	printf("\ntime = %lf", time);

	printVectorToFile(b, n, outputVector);

	fclose(outputMatrix);
	fclose(outputVector);
	MPI_Finalize();
	delete[] value;
	delete[] colIndex;
	delete[] row;
	delete[] x;
	delete[] b;
	return 0;
}