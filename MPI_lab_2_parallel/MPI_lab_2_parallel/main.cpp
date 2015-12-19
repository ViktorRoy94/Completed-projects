#include <mpi.h>

#include <stdlib.h>
#include <time.h>

const int n = 100; 

int main(int argc, char *argv[])
{
	int *A = new int[n*n];
	int *B = new int[n*n];
	int *C = new int[n*n];
//	srand( time(NULL));
	for (int i = 0; i < n*n; i++)
	{
		A[i] = rand() % 50;
		B[i] = rand() % 50;
	}
	FILE* output ;


	//for (int i = 0; i < n*n; i++)
	//{
	//	A[i] = i + 1;
	//	B[i] = n*n - i;
	//}

	int tmp; 
	for (int i = 0; i < n - 1; i++) 
		for (int j = i+1; j < n; j++) 
		{ 
			tmp = B[i*n+j]; 
			B[i*n+j] = B[j*n+i]; 
			B[j*n+i] = tmp; 
		}

	
	MPI_Init(&argc, &argv);

	MPI_Status Status;
	
	int processRank = 0;		
	int processNumb = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);	
	MPI_Comm_size(MPI_COMM_WORLD, &processNumb);	

	int processPartSize = n / processNumb; 
	int processPartElem = processPartSize * n; 
	int *bufA = new int[processPartElem];
	int *bufB = new int[processPartElem];
	int *bufC = new int[processPartElem];

	if (processRank == 0) 
		output= fopen("output.txt","w");

	double time;
	if (processRank == 0) 
		time = MPI_Wtime();

	MPI_Scatter(A, processPartElem, MPI_INT, bufA, processPartElem, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, processPartElem, MPI_INT, bufB, processPartElem, MPI_INT, 0, MPI_COMM_WORLD);
	
	int temp = 0;
	for (int i = 0; i < processPartSize; i++) 
	{
		for (int j = 0; j < processPartSize; j++) 
		{
			for (int k = 0; k < n; k++) 
				temp += bufA[i*n+k]*bufB[j*n+k];
			bufC[i*n+j+processPartSize*processRank] = temp;
			temp = 0;
		}
	}
	
	int nextProc; int prevProc;
	for (int p = 1; p < processNumb; p++) 
	{
		nextProc = processRank + 1;
		if (processRank == processNumb-1) 
			nextProc = 0;
		
		prevProc = processRank - 1;
		if (processRank == 0) 
			prevProc = processNumb - 1;

		MPI_Sendrecv_replace(bufB, processPartElem, MPI_INT, nextProc, 0, prevProc, 0, MPI_COMM_WORLD, &Status);
		
		temp = 0;
		int index;
		for (int i = 0; i < processPartSize; i++) 
		{
			for (int j = 0; j < processPartSize; j++) 
			{
				for (int k = 0; k < n; k++) 
					temp += bufA[i*n+k]*bufB[j*n+k];
				
				if (processRank - p >= 0 ) 
					index = processRank - p;
				else 
					index = (processNumb - p + processRank);
				
				bufC[i*n+j+index*processPartSize] = temp;
				temp = 0;
			}
		}
	}
	
	MPI_Gather(bufC, processPartElem, MPI_INT, C, processPartElem, MPI_INT, 0, MPI_COMM_WORLD);

	if (processRank == 0) 
	{
		time = MPI_Wtime() - time;
		fprintf(output,"processNumb = %d \n", processNumb);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				fprintf(output,"%d ", C[i*n+j]);
			fprintf(output,"\n");
		}
		fprintf(output,"time = %lf \n", time);
	}
	delete []bufA;
	delete []bufB;
	delete []bufC;

	MPI_Finalize();
}