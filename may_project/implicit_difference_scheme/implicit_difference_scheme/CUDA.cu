#include "CUDA.cuh"
#include <math.h>
#include <stdio.h>

__device__ int flag;														

 __global__ void methodJacobi(double* value, int* col, int* rowIndex, double* x1, double* x2, double *b, double eps, double diag, int N, int *iteration, double dt, double *f) // eps - точность вычислений, до которых выполняет метод
	{
		double *tmp;
		int iter = 0;														// к-во итераций в каждом потоке
		int index; 															// индекс потока
		do{
			index = threadIdx.x + blockIdx.x * blockDim.x;					// threadIdx.x - номер потока, blockIdx.x - блока
			if( index == 0 ){												// нулевой поток устанавливает flag в 0
				flag = 0;
			}
			__syncthreads();
			while (index < N)
			{
				x2[index] = 0;
				for (int j=rowIndex[index]; j<rowIndex[index+1]; j++)		// умножение crs матрицы на вектор
					x2[index] +=value[j] * x1[col[j]];				
				x2[index] =  x1[index] + ( -x2[index] + b[index] ) / diag;  // рассчет следующего значения на основе предыдущего
				
				if (fabs(x1[index]-x2[index]) > eps){
					flag = index+1;  
				}
			 	index += blockDim.x * gridDim.x;						 // gridDim.x - размер сетки в блоках, blockDim.x - размеры блока в потоках
			 }
			tmp = x1;													//			
			x1 = x2;													// меняем предыдущия и следующие значения местами
			x2 = tmp;													//
			iter++;														// увеличиваем итерации		
			__syncthreads();

		}while ( flag  );												// если кто-то записал во флаг -> выполняем дальше
		
		index = threadIdx.x + blockIdx.x * blockDim.x;					// threadIdx.x - номер потока, blockIdx.x - блока
		while (index < N){
			b[index]=f[index]*dt+x1[index];
			index += blockDim.x * gridDim.x;						 // gridDim.x - размер сетки в блоках, blockDim.x - размеры блока в потоках
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