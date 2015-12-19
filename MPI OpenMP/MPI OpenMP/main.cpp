#include <mpi.h>
#include <stdlib.h>
#include "setRandomMatrix.h"
#include "portable_time.h"
#include <omp.h>

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int processRank;
	int processNumb;

	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
	MPI_Comm_size(MPI_COMM_WORLD, &processNumb);

//	int n = 200;				// n*n размер матрицы
//	int nzInString = 3;		    // к-во ненулевых элементов в строке
//
//	if (argc > 1)
//	{
//		n = atoi(argv[1]);
//		nzInString = atoi(argv[2]);
//	}
//
//	int nz = nzInString * n;	// к-во ненулевых элементов
//
//	double *value;		// массив значений 
//	int *colIndex;		// массив координат столбцов
//	int *row;			// массив индексов строк
//
//	double *x;          // вектор
//	double *b;			// результат умножения матрицы на вектор
//
//	int processPartSize = n / processNumb;
//	int processPartElem = processPartSize * nzInString;
//
//	value = new double[processPartElem];
//	row = new int[processPartElem];
//	colIndex = new int[processPartSize + 1];
//
//	x = new double[processPartSize];
//	b = new double[n];
//
//	for (int i = 0; i < processPartSize; i++) {
//		x[i] = rand() % 100;
//	}
//
//	for (int i = 0; i < n; i++) {
//		b[i] = 0;
//	}
//
//	//FILE* output;
//	//if (processRank == 0) {
//	//	fopen_s(&output, "outputXMPI_process1.txt", "w");
//	//	printVectorToFile(x, processPartSize, output);
//	//}
//
//	double time;
//	
//	if (processRank == 0)
//		time = PortableGetTime();
//
//	setRandomMatrix(value, row, colIndex, n, nzInString, processPartSize);
//
//	/*MPI_Finalize();
//	return 0;
//*/
//	/*FILE* output;
//	if (processRank == 0) {
//		fopen_s(&output, "colMPI_process1.txt", "w");
//		printVectorToFile(colIndex, processPartSize, output);
//		fflush(stdout);
//	}*/
//	






	int n = 200;				
	int nzInString = 3;		    

	if (argc > 1)
	{
		n = atoi(argv[1]);
		nzInString = atoi(argv[2]);
	}

	int nz = nzInString * n;	

	double *value; 
	int *row;	   
	int *colIndex; 
	double *x;     
	
	double *b = new double[n];
	
	for (int i = 0; i < n; i++) {
		b[i] = 0;
	}

	int processPartSize = n / processNumb;
	int processPartElem = processPartSize * nzInString;
	
	value = new double[nz];
	row = new int[nz];
	colIndex = new int[n + 1];
	x = new double[n];

	if (processRank == 0)
	{
		setRandomMatrix(value, row, colIndex, n, nzInString, processPartSize);
		
		for (int i = 0; i < n; i++)
		{
			x[i] = rand() % 10 + 1;
		}
	}



	int    *bufCol = new int[processPartSize + 1];
	int    *bufRow = new int[processPartElem];
	double *bufX	 = new double[processPartSize];
	double *bufValue = new double[processPartElem];
	
	

	double time;
	if (processRank == 0)
		time = MPI_Wtime();

	MPI_Scatter(colIndex, processPartSize, MPI_INT, bufCol, processPartSize, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(value, processPartElem, MPI_DOUBLE, bufValue, processPartElem, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(row, processPartElem, MPI_INT, bufRow, processPartElem, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(x, processPartSize, MPI_DOUBLE, bufX, processPartSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);



	/*for (int i = 0; i < processPartSize + 1; i++ )
	{
		printf("I'm processRank = %d bufRow[%d] = %d \n", processRank, i, bufRow[i]);
		printf("I'm processRank = %d bufCol[%d] = %d \n", processRank, i, bufCol[i]);
		printf("I'm processRank = %d bufValue[%d] = %.2lf \n", processRank, i, bufValue[i]);
		printf("I'm processRank = %d x[%d] = %.2lf \n", processRank, i, x[i]);
		fflush(stdout);
	}
*/

	/*MPI_Finalize();
	return 0;
*/
	int startCol   = processRank * processPartSize;
	int endCol     = (processRank + 1)* processPartSize;
	int startValue = processRank * processPartElem;

	if (processRank == processNumb - 1) {
		endCol = n;
		bufCol [processPartSize] = nz;
	}
	
	//for (int i = startCol; i < endCol; i++) {
	//	for (int j = bufCol[i - startCol]; j < bufCol[i - startCol] + nzInString; j++) {
	//		//printf("I'm processRank = %d bufCol[%d] = %d \n", processRank, i - startCol, bufRow[i - startCol]);
	//		printf("I'm processRank = %d bufValue[%d] = %.2lf \n", processRank, j - startValue, bufValue[j - startValue]);
	//		printf("I'm processRank = %d bufRow[%d] = %d \n", processRank, i - startCol, bufRow[i - startCol]);
	//		fflush(stdout);
	//	}
	//}


	//if (processRank == processNumb - 1) {
	//	printf("I'm HERE!!!!!!!");
	//	fflush(stdout);
	//}

	for (int i = startCol; i < endCol; i++) {
		for (int j = bufCol[i - startCol]; j < bufCol[i - startCol] + nzInString; j++) {
			b[bufRow[j-startValue]] += bufValue[j-startValue] * bufX[i - startCol];

			/*printf("I'm processRank = %d bufCol[%d] = %d \n", processRank, i - startCol, bufCol [i - startCol]);
			printf("I'm processRank = %d bufRow[%d] = %d \n", processRank, i - startCol, bufRow[i - startCol]);
			printf("I'm processRank = %d b[%d] = %.2lf \n", processRank, i - startCol, b[i - startCol]);
			printf("I'm processRank = %d bufValue[%d] = %.2lf \n", processRank, j - startValue, bufValue[j - startValue]);
			fflush(stdout);*/
		}
	}



	/*FILE* output;
	if (processRank == 0) {
		fopen_s(&output, "outputVectorMPI_process1.txt", "w");
		printVectorToFile(b, n, output);
	}*/

	double* totalSum;
	totalSum = new double[n];

	MPI_Reduce(b, totalSum, n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	
	if (processRank == 0) {
		time = MPI_Wtime() - time;
		printf("Time = %lf\n", time);
	}

	FILE* output;
	if (processRank == 0) {
		fopen_s(&output, "outputVectorMPI.txt", "w");
		printVectorToFile(totalSum, n, output);
		fclose(output);
	}

	MPI_Finalize();

	delete[] value;
	delete[] colIndex;
	delete[] row;
	delete[] x;
	delete[] b;
	delete[] bufValue;
	delete[] bufCol;
	delete[] bufRow;
	delete[] bufX;
	
	return 0;
}