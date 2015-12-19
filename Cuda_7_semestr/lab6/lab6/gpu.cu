#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <curand_kernel.h>

#include "gpu.h"

#define SIZE_BLOCK 128

__global__ void Check(int n, float *x, float *y, bool *count, int a, int b)
{
	int globalIndex = blockIdx.x * blockDim.x + threadIdx.x;

	x[globalIndex] = a + x[globalIndex] * (b - a);
	if (globalIndex < n)
	{
		if (y[globalIndex] <= 1 / (1 + (x[globalIndex]) * (x[globalIndex]))) 
			count[globalIndex] = 1;
		else 
			count[globalIndex] = 0;
	}
}
__global__ void initCurand(curandStateXORWOW_t *state, int n)
{
	int globalIndex = blockIdx.x * blockDim.x + threadIdx.x;
	if (globalIndex < n) 
		curand_init(3598, globalIndex, 0, &state[globalIndex]);
}
__global__ void monteCarlo(curandStateXORWOW_t *state, int n, int a, int b, bool *res)
{
	int globalIndex = blockIdx.x * blockDim.x + threadIdx.x;

	if (globalIndex < n)
	{
		curandStateXORWOW_t localState = state[globalIndex];

		float x =  a + curand_uniform(&localState) * (b - a);
		float y = curand_uniform(&localState);

		if (y <= 1 / (1 + (x * x))) 
			res[globalIndex] = 1;
		else 
			res[globalIndex] = 0;

		state[globalIndex] = localState;
	}
}
void host(int n, float *x, float *y, bool *count, int a, int b)
{
	dim3 NUM_BLOCKS((n - 1) / SIZE_BLOCK + 1);
	dim3 BLOCK_SIZE(SIZE_BLOCK);
	Check << <NUM_BLOCKS, BLOCK_SIZE >> >(n, x, y, count, a, b);
	cudaDeviceSynchronize();
}
void gpu(int n, bool *count, int a, int b)
{
	dim3 NUM_BLOCKS((n - 1) / SIZE_BLOCK + 1);
	dim3 BLOCK_SIZE(SIZE_BLOCK);

	curandStateXORWOW_t * devStates;
	cudaMalloc ((void**)&devStates , n * sizeof(curandStateXORWOW_t));

	initCurand   <<<NUM_BLOCKS, BLOCK_SIZE >> >(devStates, n);
	monteCarlo   <<<NUM_BLOCKS, BLOCK_SIZE >> >(devStates, n, a, b, count);

	cudaDeviceSynchronize();
}