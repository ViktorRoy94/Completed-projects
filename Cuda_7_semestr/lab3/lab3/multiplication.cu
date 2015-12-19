#include "multiplication.h"

__global__ void kernel (const int * a, const int * b, int * c, int m, int n, int k)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	c[i*k + j] = 0;
	if(i < m && j < k)
    for (int k_ = 0; k_ < n; ++k_)
		c[i*k + j] += a[i*n + k_] * b[k_*k + j]; 
}


void multiplication_gpu(const int * a, const int * b, int * c, int m, int n, int k)
{
	dim3 numBlocks(m/BLOCK_SIZE, k/BLOCK_SIZE);
	dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE);

	kernel<<<numBlocks,blockSize>>>(a,  b, c, m, n,  k);
	
}

__global__ void kernel1 (const int * a, const int * b, int * c, int m, int n, int k)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	__shared__ int partA[BLOCK_SIZE][BLOCK_SIZE];
	__shared__ int partB[BLOCK_SIZE][BLOCK_SIZE];
	int result = 0;
	for(int k_ = 0; k_ < n/BLOCK_SIZE; k_++)
	{
		partA[threadIdx.x][threadIdx.y] = a[i*n + BLOCK_SIZE*k_ + threadIdx.x];
		partB[threadIdx.x][threadIdx.y] = b[(threadIdx.y + BLOCK_SIZE*k_)*n + j];
		__syncthreads();
		for (int q = 0; q < BLOCK_SIZE; ++q)
			result += partA[q][threadIdx.y] * partB[threadIdx.x][q];
		__syncthreads();
	}
	c[i*k+j] = result;
}

void multiplication_gpu_opt(const int * a, const int * b, int * c, int m, int n, int k)
{
	dim3 numBlocks(k/BLOCK_SIZE, m/BLOCK_SIZE);
	dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE);
	kernel1<<<numBlocks,blockSize>>>(a, b, c, m, n, k);
}
