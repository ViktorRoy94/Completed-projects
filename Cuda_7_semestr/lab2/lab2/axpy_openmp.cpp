#include "axpy.h"
#include <math.h>
void saxpy_openmp(int n, float a, float * x, int incx, float * y, int incy)
{
	#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
		for (int j = 0 ; j < 100; ++j)
			y[i * incy] += a * x[i * incx] * cos((float)j);
	}
}

void daxpy_openmp(int n, double a, double * x, int incx, double * y, int incy)
{
	#pragma omp parallel for
    for (int i = 0; i < n; ++i)
        y[i * incy] += a * x[i * incx];
}
