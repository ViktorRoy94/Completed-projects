#include "CUDA.cuh"
#include <math.h>
#include <stdio.h>

__device__ int flag;														

 __global__ void methodJacobi(double* value, int* col, int* rowIndex, double* x1, double* x2, double *b, double eps, double diag, int N, int *iteration, double dt, double *f) // eps - òî÷íîñòü âû÷èñëåíèé, äî êîòîðûõ âûïîëíÿåò ìåòîä
	{
		double *tmp;
		int iter = 0;														// ê-âî èòåðàöèé â êàæäîì ïîòîêå
		int index; 															// èíäåêñ ïîòîêà
		do{
			index = threadIdx.x + blockIdx.x * blockDim.x;					// threadIdx.x - íîìåð ïîòîêà, blockIdx.x - áëîêà
			if( index == 0 ){												// íóëåâîé ïîòîê óñòàíàâëèâàåò flag â 0
				flag = 0;
			}
			__syncthreads();
			while (index < N)
			{
				x2[index] = 0;
				for (int j=rowIndex[index]; j<rowIndex[index+1]; j++)		// óìíîæåíèå crs ìàòðèöû íà âåêòîð
					x2[index] +=value[j] * x1[col[j]];				
				x2[index] =  x1[index] + ( -x2[index] + b[index] ) / diag;  // ðàññ÷åò ñëåäóþùåãî çíà÷åíèÿ íà îñíîâå ïðåäûäóùåãî
				
				if (fabs(x1[index]-x2[index]) > eps){
					flag = index+1;  
				}
			 	index += blockDim.x * gridDim.x;						 // gridDim.x - ðàçìåð ñåòêè â áëîêàõ, blockDim.x - ðàçìåðû áëîêà â ïîòîêàõ
			 }
			tmp = x1;													//			
			x1 = x2;													// ìåíÿåì ïðåäûäóùèÿ è ñëåäóþùèå çíà÷åíèÿ ìåñòàìè
			x2 = tmp;													//
			iter++;														// óâåëè÷èâàåì èòåðàöèè		
			__syncthreads();

		}while ( flag  );												// åñëè êòî-òî çàïèñàë âî ôëàã -> âûïîëíÿåì äàëüøå
		
		index = threadIdx.x + blockIdx.x * blockDim.x;					// threadIdx.x - íîìåð ïîòîêà, blockIdx.x - áëîêà
		while (index < N){
			b[index]=f[index]*dt+x1[index];
			index += blockDim.x * gridDim.x;						 // gridDim.x - ðàçìåð ñåòêè â áëîêàõ, blockDim.x - ðàçìåðû áëîêà â ïîòîêàõ
		}

		if( threadIdx.x + blockIdx.x * blockDim.x == 0 ){
			*iteration = iter; 
		}

	}

void StartCuda(double* value, int* col, int* rowIndex, double* x1, double* x2, double *b, double eps, double diag, int *iteration, int Sx, int Sy, int Sz, double dt, double *f)
{
	methodJacobi<<<1,512>>>(value, col, rowIndex, x1, x2, b, eps, diag, (Sx-2)*(Sy-2)*(Sz-2), iteration,dt,f); 
	cudaDeviceSynchronize();
	//cudaError_t err;
	//err = cudaGetLastError();
	//printf("err=%d, %s\n", (int)err, cudaGetErrorString(err));
}