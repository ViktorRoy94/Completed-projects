#include "Parallel algorithm.h"

int sumMPI(int *a, int n, double &time, int argc, char *argv[])
{
	int s = 0;					// êîíå÷íàÿ ñóììà âñåõ êîìïîíåíòîâ âåêòîðà 
	int processSum  = 0;		// ñóììà ÷àñòè ýëåìåíòîâ äàííûõ ïðîöåññó
	int processRank = 0;		// íîìåð ïðîöåññà
	int processNumb = 0;		// êîë-âî ïðîöåññîâ
	int processRecv = 0;		// áóôåð äëÿ ïîëó÷åíèÿ äàííûõ îò äðóãèõ ïðîöåññîâ
	
	time = PortableGetTime();	
	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);	
	MPI_Comm_size(MPI_COMM_WORLD, &processNumb);

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
		printf("s = %d\n", s);
	}
	else MPI_Send(&processSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	if (processRank == 0) 
		printf("%d\n",s);
	
	//fflush(stdout);
	MPI_Finalize();

	time = PortableGetTime() - time;

	return s;
}