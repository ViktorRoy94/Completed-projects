#include "crsMatrix.h"
#include <stdio.h>
int main()
{
	int N=0;  // размер матрицы
	int NZ=0; // к-во ненулевых элементов
	
	printf("Size of matrix:");
	scanf("%d",&N);
	printf("Count of no zero numbers:");
	scanf("%d",&NZ);
	
	double *x = new double[N];      // вектор x длины N
	double *value = new double[NZ]; // массив значений (размер NZ)
	int *col = new int[NZ];		    // массив номеров столбцов (размер NZ)
	int *rowIndex = new int[N+1];   // массив индексов строк (размер N+1)
    double *b = new double[N];      // результат умножения матрицы на вектор
	FILE* fp = fopen("input.txt","r");
	for (int i=0; i<N; i++)	           // считываем вектор х из файла
		fscanf(fp,"%lf",&x[i]);
	fscanf(fp,"\n");

	for (int i=0; i<NZ; i++)	           // считываем массив value из файла
		fscanf(fp,"%lf",&value[i]);
	fscanf(fp,"\n");

	for (int i=0; i<NZ; i++)	           // считываем массив col из файла
		fscanf(fp,"%d",&col[i]);
	fscanf(fp,"\n");

	for (int i=0; i<N+1; i++)	           // считываем массив rowIndex из файла
		fscanf(fp,"%d",&rowIndex[i]);
	fscanf(fp,"\n");

	crsMatrix A(N, NZ, value, col, rowIndex); // создаем crs матрицу А
	
	b = A.Multiplicate(x);
	
	for (int i=0; i<N; i++)	           // вывод на экран результата
		printf("%lf\n",b[i]);

	fclose(fp);
	delete [] x;
	delete [] value;
	delete [] col;
	delete [] rowIndex;
}
