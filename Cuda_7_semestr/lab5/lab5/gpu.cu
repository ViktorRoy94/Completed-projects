#include "convolution.h"


__global__ void gpu(cufftComplex * a, cufftComplex * b, int n, cufftComplex * result)
{
	int i = blockDim.x*blockIdx.x+threadIdx.x;
	if(i < n)
	{
		result[i].x = (a[i].x*b[i].x-a[i].y*b[i].y) / n;
		result[i].y = (a[i].x*b[i].y+a[i].y*b[i].x) / n;
	}
}

void convolve_gpu(cufftComplex * a, cufftComplex * b, int n, cufftComplex * result)
{
	cufftHandle plan;
	cufftPlan1d(&plan, n, CUFFT_C2C, 1);
	cufftExecC2C(plan, a, a, CUFFT_FORWARD);
	cufftExecC2C(plan, b, b, CUFFT_FORWARD);
	dim3 numBlocks((n-1)/256+1);
	dim3 blockSize(256);
	gpu<<<numBlocks, blockSize>>>(a,  b,  n, result);

	cufftExecC2C(plan, result, result, CUFFT_INVERSE);

	cufftDestroy(plan);

}
