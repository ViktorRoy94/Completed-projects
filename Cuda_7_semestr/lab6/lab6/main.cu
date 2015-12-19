#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <curand.h>
#include <cufft.h>

#include "gpu.h"

#define n 100000 
#define a -4
#define b  4 

int main(int argc, char** argv)
{
	float integral = (float)atan(b) - (float)atan(a);

	curandGenerator_t curandGenerator;
	curandCreateGenerator(&curandGenerator, CURAND_RNG_PSEUDO_XORWOW);
	curandSetPseudoRandomGeneratorSeed(curandGenerator, 8694ULL);

	float *x, *y;
	bool *dev_count, *count;

	count  = (bool*)calloc(n, sizeof(bool));

	cudaMalloc((void**)&x,         n * sizeof(float));
	cudaMalloc((void**)&y,         n * sizeof(float));
	cudaMalloc((void**)&dev_count,  n * sizeof(bool));

	curandGenerateUniform(curandGenerator, x, n);
	curandGenerateUniform(curandGenerator, y, n);

	curandDestroyGenerator(curandGenerator);

	host(n, x, y, dev_count, a, b);

	cudaMemcpy(count, dev_count, n* sizeof(bool), cudaMemcpyDeviceToHost);

	int k = 0;
	for (int i = 0; i < n; i++) 
		if (count[i] == 1) k++;
	float hostIntegral = (float)(abs(b - a) * k) / n;
	
	gpu(n, dev_count, a, b);

	cudaMemcpy(count, dev_count, n * sizeof(bool), cudaMemcpyDeviceToHost);

	k = 0;
	for (int i = 0; i < n; i++) 
		if (count[i] == 1) k++;
	float monteCarloIntegral = (float)(abs(b - a) * k) / n;

	printf("default     %f\nhost        %f\nMonte Carlo %f\n", integral, hostIntegral, monteCarloIntegral);

	system("pause");
	return 0;
}



