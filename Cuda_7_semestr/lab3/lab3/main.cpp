#include <cstdlib>
#include <iomanip>
#include <iostream>


#include <windows.h>
#include <cuda_runtime_api.h>

#include "multiplication.h"

int main()
{
    const int m = BLOCK_SIZE * 52, n = BLOCK_SIZE * 58, k = BLOCK_SIZE * 54;
    const int iterations = 2;

    int * a = new int[m * n];
	int * b = new int[n * k];
	int * c = new int[m * k];
    int * c_verify = new int[m * k];

    int * dev_a;
	int * dev_b;
    int * dev_c;
	
	cudaMalloc((void **) &dev_a, m * n * sizeof(int));
    cudaMalloc((void **) &dev_b, n * k * sizeof(int));
    cudaMalloc((void **) &dev_c, m * k * sizeof(int));

    // random fill
    for (int i = 0; i < m * n; ++i)
        a[i] = int(rand()) / RAND_MAX;
    for (int i = 0; i < n * k; ++i)
        b[i] = int(rand()) / RAND_MAX;

    // copy to gpu
    cudaMemcpy(dev_a, a, m * n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, n * k * sizeof(int), cudaMemcpyHostToDevice);

    clock_t start;
	clock_t finish;
	std::cout << std::fixed << std::setprecision(5);

    // CPU
	start = clock();
    for (int i = 0; i < iterations; ++i)
        multiplication(a, b, c, m, n, k);
    finish = clock();

    double cpu_time = (finish - start) / (double)CLOCKS_PER_SEC;
    std::cout << "CPU " << cpu_time << " seconds.\n\n";

    // GPU first version
    start = clock();
    for (int i = 0; i < iterations; ++i)
        multiplication_gpu(dev_a, dev_b, dev_c, m, n, k);
    cudaThreadSynchronize();
    finish = clock();

	double gpu_time = (finish - start) / (double)CLOCKS_PER_SEC;
    std::cout << "GPU first version " << gpu_time << " seconds.\n";

    cudaMemcpy(c_verify, dev_c, m * k * sizeof *dev_c, cudaMemcpyDeviceToHost);

    double diff = 0;
    for (int i = 0; i < m * k; ++i)
        diff += (c[i] - c_verify[i]) * (c[i] - c_verify[i]);
    if (diff < 1e-4f)
        std::cout << "Correct.\n";
    else
        std::cout << "Incorrect.\n";
    std::cout << "\n";

    cudaMemset(dev_c, 0, m * k * sizeof(int));

    // GPU second version
    start = clock();
    for (int i = 0; i < iterations; ++i)
        multiplication_gpu_opt(dev_a, dev_b, dev_c, m, n, k);
    cudaThreadSynchronize();
    finish = clock();

    double opt_gpu_time = (finish - start) /(double) CLOCKS_PER_SEC;
    std::cout << "GPU second version " << opt_gpu_time << " seconds.\n";

    cudaMemcpy(c_verify, dev_c, m * k * sizeof *dev_c, cudaMemcpyDeviceToHost);

    diff = 0;
    for (int i = 0; i < m * k; ++i)
        diff += (c[i] - c_verify[i]) * (c[i] - c_verify[i]);
    if (diff < 1e-4f)
        std::cout << "Correct.\n";
    else
        std::cout << "Incorrect.\n";
	
	system("pause");
	
	cudaFree(dev_c);
	cudaFree(dev_b);
	cudaFree(dev_a);
 
	delete [] a;
	delete [] b;
	delete [] c;
	delete [] c_verify;
}

