#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <time.h>

#include <windows.h>
#include <cuda_runtime_api.h>

#include "axpy.h"

int main()
{
    const int n = 1000000;
    const int incx = 2;
    const int incy = 3;
    const float a = float(rand()) / RAND_MAX;

    float * x, * y, * dev_x, * dev_y, * y_verify;

    x = new float[n * incx];
	y = new float[n * incy];
	y_verify = new float[n * incy];
    
	cudaMalloc((void **) &dev_x, n * incx * sizeof(float));
    cudaMalloc((void **) &dev_y, n * incy * sizeof(float));

    for (int i = 0; i < n * incx; ++i)
        x[i] = float(rand()) / RAND_MAX;
    for (int i = 0; i < n * incy; ++i)
        y[i] = float(rand()) / RAND_MAX;

    cudaMemcpy(dev_x, x, n * incx * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_y, y, n * incy * sizeof(float), cudaMemcpyHostToDevice);

	clock_t start = clock();
    saxpy(n, a, x, incx, y, incy);
    clock_t finish = clock();

	std::cout.setf(std::ios_base::fixed);
    std::cout.precision(5);

    double cpu_time = (finish - start) /(double) CLOCKS_PER_SEC;
    std::cout << "CPU " << cpu_time << " seconds.\n";

	start = clock();
    saxpy_openmp(n, a, x, incx, y, incy);
    finish = clock();
	double cpu_openmp_time = (finish - start) /(double) CLOCKS_PER_SEC;
    std::cout << "CPU_openmp " << cpu_openmp_time << " seconds.\n";

	start = clock();
    saxpy_gpu(n, a, dev_x, incx, dev_y, incy);
    cudaThreadSynchronize();
    finish = clock();

    double gpu_time = (float) (finish - start) / (double)CLOCKS_PER_SEC;

    std::cout << "GPU " << gpu_time << " seconds.\n";

    cudaMemcpy(y_verify, dev_y, n * incy * sizeof(float), cudaMemcpyDeviceToHost);

    double diff = 0;
    for (int i = 0; i < n * incy; ++i)
        diff += (y[i] - y_verify[i]) * (y[i] - y_verify[i]);

    if (diff < 1e-4f)
        std::cout << "Correct.\n";
    else
        std::cout << "Correct.\n";
	
	////
	//// а теперь тоже самое для double
	////

	//double * xd, * yd, * dev_xd, * dev_yd, * y_verifyd;
	//const float ad = float(rand()) / RAND_MAX;

 //   xd = new double[n * incx];
	//yd = new double[n * incy];
	//y_verifyd = new double[n * incy];
 //   
	//cudaMalloc((void **) &dev_xd, n * incx * sizeof(double));
 //   cudaMalloc((void **) &dev_yd, n * incy * sizeof(double));

 //   for (int i = 0; i < n * incx; ++i)
 //       xd[i] = double(rand()) / RAND_MAX;
 //   for (int i = 0; i < n * incy; ++i)
 //       yd[i] = double(rand()) / RAND_MAX;

 //   cudaMemcpy(dev_xd, xd, n * incx * sizeof(double), cudaMemcpyHostToDevice);
 //   cudaMemcpy(dev_yd, yd, n * incy * sizeof(double), cudaMemcpyHostToDevice);

	//start = clock();
 //   for (int i = 0; i < iterations; ++i)
 //       daxpy(n, ad, xd, incx, yd, incy);
 //   finish = clock();

 //   cpu_time = (finish - start) / CLOCKS_PER_SEC;
 //   std::cout << "CPU " << cpu_time << " seconds.\n";

	//start = clock();
 //   for (int i = 0; i < iterations; ++i)
 //       daxpy_gpu(n, ad, dev_xd, incx, dev_yd, incy);
 //   
 //   cudaThreadSynchronize();
 //   finish = clock();

 //   gpu_time = (float) (finish - start) / CLOCKS_PER_SEC;

 //   std::cout << "GPU " << gpu_time << " seconds.\n";

 //   cudaMemcpy(y_verifyd, dev_yd, n * incy * sizeof(double), cudaMemcpyDeviceToHost);

	//diff = 0;
 //   for (int i = 0; i < n * incy; ++i)
 //       diff += (yd[i] - y_verifyd[i]) * (yd[i] - y_verifyd[i]);

 //   if (diff < 1e-1f)
 //       std::cout << "Correct.\n";
 //   else
 //       std::cout << "Incorrect.\n";


	system("pause");

	cudaFree(dev_x);
    cudaFree(dev_y);

	//cudaFree(dev_xd);
 //   cudaFree(dev_yd);
	
    delete [] x;
	delete [] y;
	delete [] y_verify;

	//delete [] xd;
	//delete [] yd;
	//delete [] y_verifyd;
}
