#include <stdio.h>
#include <mpi.h>
#include <Windows.h>
#include "portable_time.h"

int main (int argc, char *argv[])
{
	MPI_Init(&argc, &argv);		

	int *a;						// ������ ������
	int n;						// �-�� ��-��� �������
	int s = 0;					// �������� ����� ���� ����������� ������� 
	int processSum  = 0;		// ����� ����� ��������� ������ ��������
	int processRank = 0;		// ����� ��������
	int processNumb = 0;		// ���-�� ���������
	int processRecv = 0;		// ����� ��� ��������� ������ �� ������ ���������
	double time;				// ����� ������ ���������
	MPI_Status status;			
	
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);	// ������ ����� ��������  
	MPI_Comm_size(MPI_COMM_WORLD, &processNumb);	// ������ ���-�� ���������
	
	FILE* input  = fopen("input.txt","r");			// ���� �������� ������ 
	FILE* output = fopen("output.txt","w");			// ���� �������� ������
	
	fscanf(input, "%d", &n);						// ���������� ���-�� ���������

	a = new int[n];
	for (int i = 0; i < n; i++)					//
	{											// ���������� �������
		fscanf(input,"%d",&a[i]);				// ���������� �� �����
	}											//

	time = PortableGetTime();	
	
	int k = n/processNumb;
	int start  = k * processRank;
	int finish = k * (processRank + 1);
	if (processRank == (processNumb-1)) finish = n;
	for (int i = start; i < finish; i++)
		processSum += a[i];

	if (processRank == 0) 
	{
		s = processSum;
		for (int i = 1; i < processNumb; i++)
		{
			MPI_Recv(&processRecv, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			s +=processRecv;
		}
	}
	else MPI_Send(&processSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	
	//fflush(stdout);
	MPI_Finalize();

	time = PortableGetTime() - time;

	if (processRank == 0)
	{
		fprintf(output,"--------------------------\n");
		fprintf(output,"s = %d\n", s);
		fprintf(output,"time = %f\n", time);
		fprintf(output,"--------------------------\n");
	}
	
	fclose(input);
	fclose(output);
	return 0;
}