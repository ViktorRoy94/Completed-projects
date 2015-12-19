#include <cuda_runtime_api.h>
#include "crsMatrix.h"

__global__ void methodJacobi(double* value, int* col, int* rowIndex, double* x1, double* x2, double *b, double eps, double diag, int N, int iteration, double dt, double *f);
void StartCuda(double* value, int* col, int* rowIndex, double* x1, double* x2, double *b, double eps, double diag, int *iteration, int Sx, int Sy, int Sz, double dt, double *f);