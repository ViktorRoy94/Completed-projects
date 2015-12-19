#ifndef CUDA_MONTE_KARLO_H
#define CUDA_MONTE_KARLO_H

#include <cufft.h>
#include <curand.h>

void host(int n, float *x, float *y, bool *count, int a, int b);
void gpu(int n, bool *count, int a, int b);

#endif