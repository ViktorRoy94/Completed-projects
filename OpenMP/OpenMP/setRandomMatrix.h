#include <stdio.h>
#include <string>

// функция проверки одинаковых значений в массиве
int prov(int start, int end, int *mas, int digit)
{
	for (int i = start; i < end; i++)
	if (mas[i] == digit) return 1;
	return 0;
}

void setRandomMatrix1(double *value, int *row, int *colIndex, int n, int nzInString)
{
	int nz = n * nzInString;

	// заполняем случайно вектор значений
	#pragma omp parallel for
	for (int i = 0; i < nz; i++) {
		value[i] = rand() % 10 + 1;
	}

	// заполняем вектор индексов строк
	#pragma omp parallel for
	for (int i = 0; i < n + 1; i++)
	{
		colIndex[i] = i * nzInString;
	}

	// св-ва массива colIndex
	colIndex[0] = 0;
	colIndex[n] = nz;

	// заполняем row неповторяющимися значениями в j строке 
	memset(row, 0, nz*sizeof(int));
	#pragma omp parallel for
	for (int j = 0; j < n; j++)
	{
		for (int k = colIndex[j]; k < colIndex[j + 1]; k++)
		{
			do
			{
				int R1 = rand() % n;
				int R2 = rand() % n;
				double temp = (double)(R1 * R2) / (RAND_MAX * RAND_MAX);
				row[k] = (int)(temp * n);
			} while (prov(colIndex[j], k, row, row[k]));
		}
	}

	// выстраиваем значения row по возрастанию в каждой j строке
	for (int i = 0; i < n; i++)
	{
		for (int k = colIndex[i]; k < colIndex[i + 1]; k++)
		{
			for (int j = colIndex[i + 1] - 1; j > k; j--)
			{
				if (row[j - 1] > row[j])
				{
					int tmp = row[j];
					row[j] = row[j - 1];
					row[j - 1] = tmp;
				}
			}
		}
	}
	
}

void printVectorToFile(const double *b, int N, FILE* output)
{
	for (int i = 0; i < N; i++)
	{
		fprintf(output, "%lf\n", b[i]);
	}
}

void printVectorToFile(const int *b, int N, FILE* output)
{
	for (int i = 0; i < N; i++)
	{
		fprintf(output, "%d\n", b[i]);
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