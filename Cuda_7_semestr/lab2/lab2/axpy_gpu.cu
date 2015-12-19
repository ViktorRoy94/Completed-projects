#include "axpy.h"

__global__ void summ_float(int n, float a, float * x, int incx, float * y, int incy)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < n) {
		for (int j = 0 ; j < 100; ++j)
			y[i * incy] += a * x[i * incx] * cos((float)j);	
	}
}

void saxpy_gpu(int n, float a, float * x, int incx, float * y, int incy)
{
	const int block_size = 128;
	int num_blocks = (n-1)/block_size + 1;
	summ_float<<<num_blocks, block_size>>>(n, a, x, incx, y, incy);
}


__global__ void summ_double(int n, double a, double * x, int incx, double * y, int incy)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < n)
		 y[i * incy] += a * x[i * incx];
}


void daxpy_gpu(int n, double a, double * x, int incx, double * y, int incy)
{
	const int block_size = 128;
	int num_blocks = (n-1)/block_size + 1;
	summ_double<<<num_blocks, block_size>>>(n, a, x, incx, y, incy);
}