#include "CUDA.cuh"
#include <math.h>
#include <stdio.h>

__device__ int flag;														

 __global__ void methodJacobi(double* value, int* col, int* rowIndex, double* x1, double* x2, double *b, double eps, double diag, int N, int *iteration, double dt, double *f) // eps - ц╡ц╝ц╥ц╜ц╝ц╠ц╡ц╪ ц╒ц╩ц╥ц╗ц╠ц╚ц╔ц╜ц╗ц╘, ц╓ц╝ ц╙ц╝ц╡ц╝ц╟ц╩ц╣ ц╒ц╩ц╞ц╝ц╚ц╜ц©ц╔ц╡ ц╛ц╔ц╡ц╝ц╓
	{
		double *tmp;
		int iter = 0;														// ц╙-ц╒ц╝ ц╗ц╡ц╔ц╟ц═ц╤ц╗ц╘ ц╒ ц╙ц═ц╕ц╓ц╝ц╛ ц╞ц╝ц╡ц╝ц╙ц╔
		int index; 															// ц╗ц╜ц╓ц╔ц╙ц╠ ц╞ц╝ц╡ц╝ц╙ц═
		do{
			index = threadIdx.x + blockIdx.x * blockDim.x;					// threadIdx.x - ц╜ц╝ц╛ц╔ц╟ ц╞ц╝ц╡ц╝ц╙ц═, blockIdx.x - ц║ц╚ц╝ц╙ц═
			if( index == 0 ){												// ц╜цЁц╚ц╔ц╒ц╝ц╘ ц╞ц╝ц╡ц╝ц╙ цЁц╠ц╡ц═ц╜ц═ц╒ц╚ц╗ц╒ц═ц╔ц╡ flag ц╒ 0
				flag = 0;
			}
			__syncthreads();
			while (index < N)
			{
				x2[index] = 0;
				for (int j = rowIndex[index]; j < rowIndex[index+1]; j++)		// цЁц╛ц╜ц╝ц╕ц╔ц╜ц╗ц╔ crs ц╛ц═ц╡ц╟ц╗ц╤ц╩ ц╜ц═ ц╒ц╔ц╙ц╡ц╝ц╟
					x2[index] += value[j] * x1[col[j]];				
				x2[index] =  x1[index] + ( -x2[index] + b[index] ) / diag;  // ц╟ц═ц╠ц╠ц╥ц╔ц╡ ц╠ц╚ц╔ц╓цЁц╬ц╧ц╔цёц╝ ц╖ц╜ц═ц╥ц╔ц╜ц╗ц© ц╜ц═ ц╝ц╠ц╜ц╝ц╒ц╔ ц╞ц╟ц╔ц╓ц╩ц╓цЁц╧ц╔цёц╝
				
				if (fabs(x1[index]-x2[index]) > eps){
					flag = index+1;  
				}
			 	index += blockDim.x * gridDim.x;						 // gridDim.x - ц╟ц═ц╖ц╛ц╔ц╟ ц╠ц╔ц╡ц╙ц╗ ц╒ ц║ц╚ц╝ц╙ц═ц╣, blockDim.x - ц╟ц═ц╖ц╛ц╔ц╟ц╩ ц║ц╚ц╝ц╙ц═ ц╒ ц╞ц╝ц╡ц╝ц╙ц═ц╣
			 }
			tmp = x1;													//			
			x1 = x2;													// ц╛ц╔ц╜ц©ц╔ц╛ ц╞ц╟ц╔ц╓ц╩ц╓цЁц╧ц╗ц© ц╗ ц╠ц╚ц╔ц╓цЁц╬ц╧ц╗ц╔ ц╖ц╜ц═ц╥ц╔ц╜ц╗ц© ц╛ц╔ц╠ц╡ц═ц╛ц╗
			x2 = tmp;													//
			iter++;														// цЁц╒ц╔ц╚ц╗ц╥ц╗ц╒ц═ц╔ц╛ ц╗ц╡ц╔ц╟ц═ц╤ц╗ц╗		
			__syncthreads();

		}while ( flag  );												// ц╔ц╠ц╚ц╗ ц╙ц╡ц╝-ц╡ц╝ ц╖ц═ц╞ц╗ц╠ц═ц╚ ц╒ц╝ ц╢ц╚ц═цё -> ц╒ц╩ц╞ц╝ц╚ц╜ц©ц╔ц╛ ц╓ц═ц╚ц╪ц╦ц╔
		
		index = threadIdx.x + blockIdx.x * blockDim.x;					// threadIdx.x - ц╜ц╝ц╛ц╔ц╟ ц╞ц╝ц╡ц╝ц╙ц═, blockIdx.x - ц║ц╚ц╝ц╙ц═
		while (index < N){
			b[index]=f[index]*dt+x1[index];
			index += blockDim.x * gridDim.x;						 // gridDim.x - ц╟ц═ц╖ц╛ц╔ц╟ ц╠ц╔ц╡ц╙ц╗ ц╒ ц║ц╚ц╝ц╙ц═ц╣, blockDim.x - ц╟ц═ц╖ц╛ц╔ц╟ц╩ ц║ц╚ц╝ц╙ц═ ц╒ ц╞ц╝ц╡ц╝ц╙ц═ц╣
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