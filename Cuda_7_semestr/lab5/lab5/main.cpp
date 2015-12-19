#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "math.h"

#include <cuda_runtime_api.h>

#include "convolution.h"

int main()
{
    const int n = 1000; 
    cufftComplex * a;
	cufftComplex * b;
	cufftComplex * a_gpu;
	cufftComplex * b_gpu;
	cufftComplex * result;
	cufftComplex * result_gpu;
    cufftComplex * result_verify;

    a = new cufftComplex[n];
    b = new cufftComplex[n];
	result = new cufftComplex[n];
    result_verify = new cufftComplex[n];

    for (int i = 0; i < n; ++i)
    {
        a[i].x = sin(6.28f * (float)i / n);
        a[i].y = cos(6.28f * (float)i / n);
        b[i].x = cos(6.0f * (float)i / n + 1);
        b[i].y = sin(6.0f * (float)i / n + 1);
    }

    cudaMalloc((void **) &a_gpu, n * sizeof *a_gpu);
    cudaMalloc((void **) &b_gpu, n * sizeof *b_gpu);
    cudaMalloc((void **) &result_gpu, n * sizeof *result_gpu);
    cudaMemcpy(a_gpu, a, n * sizeof *a_gpu, cudaMemcpyHostToDevice);
    cudaMemcpy(b_gpu, b, n * sizeof *b_gpu, cudaMemcpyHostToDevice);

    // CPU 
    convolve(a, b, n, result);

    // GPU
    convolve_gpu(a_gpu, b_gpu, n, result_gpu);
    cudaThreadSynchronize();
    cudaMemcpy(result_verify, result_gpu, n * sizeof *result_gpu,
        cudaMemcpyDeviceToHost);

    float diff = 0;
    for (int i = 0; i < n; ++i)
        diff += (result[i].x - result_verify[i].x) * (result[i].x - result_verify[i].x) +
                      (result[i].y - result_verify[i].y) * (result[i].y - result_verify[i].y);

	std::cout << "diff = " << diff << std::endl;
    if (diff < 1e-4f) 
		std::cout << "Correct.\n";
    else
        std::cout << "Incorrect.\n";

    delete [] a;
    delete [] b;
    delete [] result;
    delete [] result_verify;
    cudaFree(a_gpu);
    cudaFree(b_gpu);
    cudaFree(result_gpu);
	system("pause");
}
