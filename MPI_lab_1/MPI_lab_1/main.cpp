#include <stdio.h>
#include <mpi.h>
#include <Windows.h>
#include "portable_time.h"

int main (int argc, char *argv[])
{
	MPI_Init(&argc, &argv);		

	int *a;						// данные массив
	int n;						// к-во эл-тов массива
	int s = 0;					// конечная сумма всех компонентов вектора 
	int processSum  = 0;		// сумма части элементов данных процессу
	int processRank = 0;		// номер процесса
	int processNumb = 0;		// кол-во процессов
	int processRecv = 0;		// буфер для получения данных от других процессов
	double time;				// время работы алгоритма
	MPI_Status status;			
	
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);	// узнаем номер процесса  
	MPI_Comm_size(MPI_COMM_WORLD, &processNumb);	// узнаем кол-во процессов
	
	FILE* input  = fopen("input.txt","r");			// файл исходных данных 
	FILE* output = fopen("output.txt","w");			// файл выходных данных
	
	fscanf(input, "%d", &n);						// считывание кол-ва элементов

	a = new int[n];
	for (int i = 0; i < n; i++)					//
	{											// заполнение массива
		fscanf(input,"%d",&a[i]);				// значениями из файла
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