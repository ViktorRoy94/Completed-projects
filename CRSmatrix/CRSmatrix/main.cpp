#include "crsMatrix.h"
#include <stdio.h>
int main()
{
	int N=0;  // ������ �������
	int NZ=0; // �-�� ��������� ���������
	
	printf("Size of matrix:");
	scanf("%d",&N);
	printf("Count of no zero numbers:");
	scanf("%d",&NZ);
	
	double *x = new double[N];      // ������ x ����� N
	double *value = new double[NZ]; // ������ �������� (������ NZ)
	int *col = new int[NZ];		    // ������ ������� �������� (������ NZ)
	int *rowIndex = new int[N+1];   // ������ �������� ����� (������ N+1)
    double *b = new double[N];      // ��������� ��������� ������� �� ������
	FILE* fp = fopen("input.txt","r");
	for (int i=0; i<N; i++)	           // ��������� ������ � �� �����
		fscanf(fp,"%lf",&x[i]);
	fscanf(fp,"\n");

	for (int i=0; i<NZ; i++)	           // ��������� ������ value �� �����
		fscanf(fp,"%lf",&value[i]);
	fscanf(fp,"\n");

	for (int i=0; i<NZ; i++)	           // ��������� ������ col �� �����
		fscanf(fp,"%d",&col[i]);
	fscanf(fp,"\n");

	for (int i=0; i<N+1; i++)	           // ��������� ������ rowIndex �� �����
		fscanf(fp,"%d",&rowIndex[i]);
	fscanf(fp,"\n");

	crsMatrix A(N, NZ, value, col, rowIndex); // ������� crs ������� �
	
	b = A.Multiplicate(x);
	
	for (int i=0; i<N; i++)	           // ����� �� ����� ����������
		printf("%lf\n",b[i]);

	fclose(fp);
	delete [] x;
	delete [] value;
	delete [] col;
	delete [] rowIndex;
}
