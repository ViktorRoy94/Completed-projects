
// Подключаются необходимые загловочные файлы
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"

#include <stdlib.h>
#include "setRandomMatrix.h"
#include <omp.h>
#include <limits.h>
#include <mpi.h>


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int n = 100000;			
	int nzInString = 100;		    

	if (argc > 1)
	{
		n = atoi(argv[1]);
		nzInString = atoi(argv[2]);
	}
	CCSmatrix a(n, nzInString);

	a.setRandomMatrix();

	tbb::task_scheduler_init init;
	
	double time = MPI_Wtime();
	
	tbb::parallel_reduce(tbb::blocked_range<int>(0, n), a);
	//tbb::parallel_for(tbb::blocked_range<int>(0, n), CCSmatrix());
	time = MPI_Wtime() - time;

	init.terminate();
	printf("\ntime = %lf", time);


	FILE *outputVector;
	fopen_s(&outputVector, "outputVector.txt", "w");
	a.printVectorToFile(outputVector);
	fclose(outputVector);
	
	MPI_Finalize();
	
	return 0;
}