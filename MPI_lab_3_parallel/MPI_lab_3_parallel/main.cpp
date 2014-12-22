#include <stdlib.h>
#include "crsMatrix.h"
#include <mpi.h>

int main (int argc, char **argv)
{
	MPI_Init(&argc,&argv);
	int processRank;
	int processNumb;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);	
	MPI_Comm_size(MPI_COMM_WORLD, &processNumb);	
	

	//FILE* outputMatrix;
	//FILE* outputVector;

	//if ( processRank == 0) {
	//	fopen_s(&outputMatrix, "outputMatrix.txt", "w");
	//	fopen_s(&outputVector, "outputVector.txt", "w");
	//}

	int n  = 200;				// n*n размер матрицы
	int nzInString = 3;		    // к-во ненулевых элементов в строке
	
	if (argc > 1)	
	{
		n = atoi(argv[1]);
		nzInString = atoi(argv[2]);
	}
	
	int nz = nzInString * n;	// к-во ненулевых элементов

	double *value; // массив значений 
	int *col;	   // массив координат столбцов
	int *rowIndex; // массив индексов строк
	double *x;     // вектор умножения  
	double *b;	   // результат умножения
	
	x = new double[n];

	if (processRank == 0 )
	{
		
		b = new double[n];		
		value = new double[nz];
		col = new int[nz];
		rowIndex = new int[n+1];
	
		// заполняем матрицу
		setRandomMatrix1(value, col, rowIndex, n, nzInString);
//		printMatrixToFile(value, col, rowIndex, n, outputMatrix);
		
		// заполняем вектор
		for (int i = 0; i < n; i++)
		{
			x[i] = rand() % 10 + 1;
		}	
	}

	//fprintf(outputConsole, "processRank = %d\n sucesses fill",processRank);
	//fflush(stdout);


	// размер части массива для каждого процесса
	int processPartSize = n / processNumb; 
	
	// размер частей массивов Value, col
	int processPartElem = processPartSize * nzInString; 
	
	int    *bufRow   = new int[processPartSize + 1];
	double *bufB = new double[processPartSize ];

	double *bufValue = new double[processPartElem];
	int    *bufCol   = new int[processPartElem];

	double time;
	if (processRank == 0) 
		time = MPI_Wtime();

	
	MPI_Scatter(rowIndex, processPartSize, MPI_INT, bufRow, processPartSize, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(value, processPartElem, MPI_DOUBLE, bufValue, processPartElem, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(col, processPartElem, MPI_INT, bufCol, processPartElem, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	//for (int i = 0; i < processPartSize + 1; i++ )
	//{
	//	//printf("I'm processRank = %d bufRow[%d] = %d \n", processRank, i, bufRow[i]);
	//	//printf("I'm processRank = %d bufCol[%d] = %d \n", processRank, i, bufCol[i]);
	//	//printf("I'm processRank = %d bufValue[%d] = %.2lf \n", processRank, i, bufValue[i]);
	//	//printf("I'm processRank = %d x[%d] = %.2lf \n", processRank, i, x[i]);
	//	//fflush(stdout);
	//}

	int startRow = processRank * processPartSize;
	int endRow = (processRank + 1)* processPartSize;
	int startValue = processRank * processPartElem;
	
	// костыль, т.к. передаем массив rowIndex размера n, а не n+1
	if (processRank == processNumb - 1) {
		endRow = n;
		bufRow[processPartSize] = nz;
		//printf("I'm processRank = %d endRow = %d, bufRow[%d] = %d\n", processRank, endRow, endRow-3, bufRow[endRow-3] );
	}	

	/*printf("I'm processRank = %d startRow = %d, endRow = %d\n", processRank, startRow, endRow);
	fflush(stdout);*/

	/*fprintf(outputConsole, "Process %d  start = %d end = %d \n", processRank, startRow, endRow);
	fflush(stdout);*/

	for (int i = startRow; i < endRow; i++)
	{
		bufB[i - startRow] = 0;
		//printf("processRank = %d bufB[%d] = %.2lf\n",processRank, i-startRow, bufB[i-startRow]);
		for (int k = bufRow[i - startRow]; k < bufRow[i - startRow] + nzInString; k++) {
			bufB [i - startRow] += bufValue[k - startValue] * x[bufCol[k - startValue]];
			//printf("I'm processRank = %d bufRow[%d] = %d \n", processRank, i - startRow, bufRow[i - startRow]);
			//printf("I'm processRank = %d bufCol[%d] = %d \n", processRank, i - startRow, bufCol[i - startRow]);
			//printf("I'm processRank = %d bufB[%d] = %.2lf \n", processRank, i - startRow, bufB[i - startRow]);
			//printf("I'm processRank = %d bufValue[%d] = %.2lf \n", processRank, k-startValue, bufValue[k-startValue]);
			//fflush(stdout);
		}



		//printf("I'm processRank = %d bufRow[%d] = %d \n", processRank, i - startRow, bufRow[i - startRow]);
		//printf("I'm processRank = %d bufCol[%d] = %d \n", processRank, i, bufCol[i]);
		//printf("I'm processRank = %d bufValue[%d] = %.2lf \n", processRank, i, bufValue[i]);
		//printf("I'm processRank = %d x[%d] = %.2lf \n", processRank, i, x[i]);
		//fflush(stdout);

		//printf("processRank = %d bufB[%d] = %.2lf\n",processRank, i-startRow, bufB[i-startRow]);

		//if ( processRank == 0) {
		//	fprintf(outputConsole, "processRank = %d bufB[%d] = %.2lf\n",processRank, i-startRow, bufB[i-startRow]);
		//	fflush(stdout);
		//}
		//if ( processRank == 1) {
		//	fprintf(outputConsole1, "processRank = %d bufB[%d] = %.2lf\n",processRank, i-startRow, bufB[i-startRow]);
		//	fflush(stdout);
		//}
	}


	//for (int i = 0; i < processPartSize + 1; i++ )
	//{
	//	printf("I'm processRank = %d bufB[%d] = %.2lf \n", processRank, i, bufB[i]);
	//	fflush(stdout);
	//}


	//printf("Process %d successed bufB     \n", processRank);
	//fflush(stdout);

	MPI_Gather(bufB, processPartSize, MPI_DOUBLE, b, processPartSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	//printf( "Process %d successed gather     \n", processRank);
	//fflush(stdout);

	if (processRank == 0) {
		time = MPI_Wtime() - time;
//		printVectorToFile(b, n, outputVector);
		printf("time = %lf \n", time);
	}

	/*printf( "Process %d successed output     \n", processRank);*/
	/*fflush(stdout);*/

	
	MPI_Finalize();

	delete [] bufValue;
	delete [] bufCol;
	delete [] bufRow;
	delete [] bufB;
	delete [] x;
	delete [] b;
	
	if (processRank == 0) {
		//fclose(outputMatrix);
		//fclose(outputVector);
		delete [] value;
		delete [] col;
		delete [] rowIndex;
	}
	
}