#include <cuda_runtime_api.h>
#include "crsMatrix.h"

__global__ int methodJacobi(crsMatrix A, double* x1, double* x2, double *b, double eps, double diag);
void StartCuda(crsMatrix A, double* x1, double* x2, double *b, double eps, double diag);