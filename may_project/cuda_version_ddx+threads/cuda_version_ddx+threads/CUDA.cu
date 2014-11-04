#include <stdio.h>
#include "CUDA.cuh"
__device__ double function(double x, double y, double z)
{
    double fxyz=0.0;
    if (((x>=5.0) && (x<=5.2)) && ((y>=5.0) && (y<=5.2)) && ((z>=5.0) && (z<=5.2)))
        fxyz=4.0;
    return fxyz;
}

__global__ void kernelMatrix(double* masprev, double* masnext,int Sx, int Sy, int Sz,double dx, double dy, double dz,double x0,double y0,double z0,double dt,double ddx,double ddy,double ddz)
{
	int x = threadIdx.x+1;
	int y = blockIdx.x+1;
    for(int z=1; z<Sx-1; z++)
    {
		masnext[x+y*Sx+z*Sx*Sy]=
			dt*((masprev[(x+1)+y*Sx+z*Sx*Sy]-2*masprev[x+y*Sx+z*Sx*Sy]+masprev[(x-1)+y*Sx+z*Sx*Sy])/(dx*dx)
			+(masprev[x+(y+1)*Sx+z*Sx*Sy]-2*masprev[x+y*Sx+z*Sx*Sy]+masprev[x+(y-1)*Sx+z*Sx*Sy])/(dy*dy)
			+(masprev[x+y*Sx+(z+1)*Sx*Sy]-2*masprev[x+y*Sx+z*Sx*Sy]+masprev[x+y*Sx+(z-1)*Sx*Sy])/(dz*dz)
			+function(x0+dx*x, y0+dy*y, z0+dz*z)-masprev[x+y*Sx+z*Sx*Sy])+masprev[x+y*Sx+z*Sx*Sy];
    }
}


void StartCuda(double* masprev, double* masnext,int Sx, int Sy, int Sz,double dx, double dy, double dz,double x0,double y0,double z0,double dt,double ddx,double ddy,double ddz)
{
	kernelMatrix<<<Sy-2,Sz-2>>>(masprev,masnext,Sx,Sy,Sz,dx,dy,dz,x0,y0,z0,dt,ddx,ddy,ddz);
	cudaDeviceSynchronize();
}


