#include <cuda_runtime_api.h>

__global__ void kernelMatrix(double* masprev, double* masnext,int Sx, int Sy, int Sz,double dx, double dy, double dz,double x0,double y0,double z0,double dt,double ddx,double ddy,double ddz);
void StartCuda(double* masprev, double* masnext,int Sx, int Sy, int Sz,double dx, double dy, double dz,double x0,double y0,double z0,double dt,double ddx,double ddy,double ddz);
__device__ double f(double x, double y, double z);