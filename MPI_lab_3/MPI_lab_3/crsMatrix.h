#include <stdio.h>
#include <string>

int prov(int start, int end, int *mas, int digit);


void Multiplicate(const double *value, const int *col, const int *rowIndex, double *x , double *b, int N)
{
	for ( int i=0; i<N; i++)
	{
		b[i] = 0;
		for (int j=rowIndex[i]; j<rowIndex[i+1]; j++)
			b[i] +=value[j] * x[col[j]];
	}
	return;
}
	
void printVector (const double *b, int N)
{
	for (int i = 0; i < N; i++)
	{
		printf ("%lf\n", b[i]);
	}
}

void printVectorToFile (const double *b, int N, FILE* output)
{
	for (int i = 0; i < N; i++)
	{
		fprintf (output, "%lf\n", b[i]);
	}
}

void printMatrix (const double *value, const int *col, const int *rowIndex, int N)
{
	for (int i = 0; i < N; i++) {

		if (rowIndex[i] == rowIndex[i+1]) {
				for (int j = 0; j < N; j++){
					printf("0.00 ");
				}
			}

		for (int k = rowIndex[i]; k < rowIndex[i + 1]; k++) {

			if (k == rowIndex[i]) {
				for (int j = 0; j < col[k]; j++){
					printf("0.00 ");
				}
			}

			printf("%.2lf ", value[k]);

			if (k == rowIndex[i+1]-1) {
				for (int j = col[k]; j < N-1; j++){
					printf("0.00 ");
				}
                continue;
			}

			for (int j = col[k]+1; j < col[k+1]; j++) {
				printf("0.00 ");
			}
		}
		printf("\n");
	}
}

void printMatrixToFile(const double *value, const int *col, const int *rowIndex, int N, FILE *output)
{
	for (int i = 0; i < N; i++)
	{
		for (int k = rowIndex[i]; k < rowIndex[i + 1]; k++)
		{

			if (k == rowIndex[i]) {
				for (int j = 0; j < col[k]; j++){
					fprintf(output, "0.00 ");
				}
			}

			fprintf(output, "%.2lf ", value[k]);

       			if (k == rowIndex[i + 1] - 1) {
                for (int j = col[k]; j < N - 1; j++){
					fprintf(output, "0.00 ");
				}
                continue;
			}
            for (int j = col[k] + 1; j < col[k + 1]; j++) {
				fprintf(output, "0.00 ");
			}
		}
		fprintf(output, "\n");
	}
}

void checkedSolution (const double *value, const int *col, const int *rowIndex, const double *x, int N, int NZ)
{
	FILE *outputVectorChecked;   // для вывода решения без crs матрицы
	FILE *outputMatrixChecked;   // для вывода crs матрицы
	
	fopen_s(&outputVectorChecked, "outputVectorChecked.txt", "w");   
	fopen_s(&outputMatrixChecked, "outputMatrixChecked.txt", "w");   	
	
	double *A = new double[N*N];
	memset(A, 0, N*N*sizeof(double));	

	for (int i = 0; i < N; i++)	{
		for (int k = rowIndex[i]; k < rowIndex[i + 1]; k++) {
			A[i*N + col[k]] = value[k];
		}
	}
	
	for (int i = 0; i < N; i++)	{
		for(int j = 0; j < N; j++) {
			fprintf(outputMatrixChecked,"%.2f ", A[i * N + j]);
		}
		fprintf(outputMatrixChecked,"\n");
	}

	double *b = new double[N];			// результат умножения матрицы на вектор
	memset(b, 0, N*sizeof(double));	

	for (int i = 0; i < N; i++)	{
		for(int j = 0; j < N; j++) {
			b[i] += A[i * N + j] * x[j];
		}
	}

	printVectorToFile(b, N, outputVectorChecked);
	
	fclose(outputMatrixChecked);
	fclose(outputVectorChecked);
	delete [] A;
	
}

void setRandomMatrix (double *value, int *col, int *rowIndex, int n, int nz)
{
	// заполняем случайно вектор значений
	for(int i = 0; i < nz; i++) {
		value[i] = rand() % 10 + 1;
	}

	// заполняем rowIndex неповторящимися значениями 
	for(int i = 0; i < n + 1; i++) {
		do
		{
			rowIndex[i] = rand() % (nz);
			
		}
		while (prov(0, i, rowIndex, rowIndex[i]));
	}
	// выстраиваем значения по возрастанию
	qsort(rowIndex, n+1, sizeof(int), comp_int);
	
	// св-ва массива rowIndex
	rowIndex[0] = 0;
	rowIndex[n] = nz;
	
	// заполняем col неповторяющимися значениями в j строке 
	memset(col, 0, nz*sizeof(int));
	for (int j = 0; j < n; j++) 
	{
		for(int k = rowIndex[j]; k < rowIndex[j+1]; k++)
		{
			do
			{
				col[k] = rand() % n;
			}
			while (prov(rowIndex[j], k, col, col[k]));
		}
	}

	// выстраиваем значения col по возрастанию в каждой j строке
	for (int i = 0; i < n; i++)
	{
		for (int k = rowIndex[i]; k < rowIndex[i+1]; k++)
		{
			for (int j = rowIndex[i + 1] - 1; j > k; j--)
			{
				if (col[j - 1] > col [j])
				{
					int tmp = col[j];
					col[j] = col[j - 1];
					col[j - 1] = tmp;
				}
			}
		}
	}

}

// функция проверки одинаковых значений в массиве
int prov(int start, int end, int *mas, int digit)
{
    for(int i = start; i < end; i++)
        if (mas[i] == digit) return 1;
    return 0;
}

void setRandomMatrix1 (double *value, int *col, int *rowIndex, int n, int nzInString)
{
	int nz = n * nzInString;

	// заполняем случайно вектор значений
	for(int i = 0; i < nz; i++) {
		value[i] = rand() % 10 + 1;
	}
	
	// заполняем вектор индексов строк
	for(int i = 0; i < n + 1; i++)
	{
		rowIndex[i] = i * nzInString;
	}
	
	// св-ва массива rowIndex
	rowIndex[0] = 0;
	rowIndex[n] = nz;

	// заполняем col неповторяющимися значениями в j строке 
	memset(col, 0, nz*sizeof(int));
	for (int j = 0; j < n; j++) 
	{
		for(int k = rowIndex[j]; k < rowIndex[j+1]; k++)
		{
			do
			{
				col[k] = rand() % n;
			}
			while (prov(rowIndex[j], k, col, col[k]));
		}
	}

	// выстраиваем значения col по возрастанию в каждой j строке
	for (int i = 0; i < n; i++)
	{
		for (int k = rowIndex[i]; k < rowIndex[i+1]; k++)
		{
			for (int j = rowIndex[i + 1] - 1; j > k; j--)
			{
				if (col[j - 1] > col [j])
				{
					int tmp = col[j];
					col[j] = col[j - 1];
					col[j - 1] = tmp;
				}
			}
		}
	}
	FILE* outputrow;
	fopen_s(&outputrow, "outputcol.txt", "w");
	for (int i = 0; i < n; i++) {
		fprintf(outputrow, "row[%d] = %d\n", i, col[i]);
	}
}