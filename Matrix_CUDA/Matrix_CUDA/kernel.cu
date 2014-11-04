#include <stdio.h>
#include "kernel.cuh"
__global__ void kernelMatrixMul(int* A, int* B, int* C, int n)
{
	int i = threadIdx.x;
	int j = blockIdx.x;
	for(int k=0; k<n; k++)
	{
		C[i*n+j]+=A[i*n+k]*B[k*n+j];
	}
}

void StartCuda(int* A, int* B, int* C, int n)
{
	kernelMatrixMul<<<n, n>>>(A,B,C,n);
	cudaDeviceSynchronize();
}

