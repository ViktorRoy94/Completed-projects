#include "function.h"
#include "stdio.h"
#include "string.h"
#include "portable_time.h"
#include "CUDA.cuh"
#include <windows.h>
#include <cuda_runtime_api.h>
int main(int argc, char* argv[])
{
	double AllTime=0;
    double x0=0.0, y0=0.0, z0=0.0;
    double xn=10.0, yn=10.0, zn=10.0;
    int Sx=200, Sy=200, Sz=200, St=100;
	if (argc>1) Sx=Sy=Sz=atoi(argv[1]);
    double * masprev;
    double * masnext;
	double * f;
    masprev=new double[Sx*Sy*Sz];
    masnext=new double[Sx*Sy*Sz];
	f=new double[Sx*Sy*Sz];
    double dx=(xn-x0)/Sx, dy=(yn-y0)/Sy, dz=(zn-z0)/Sz;
   
    FILE* filex=fopen("filex.txt","w");
    FILE* filey=fopen("filey.txt","w");
    FILE* filez=fopen("filez.txt","w");	

	fprintf(filex,"%d %f %f\n",Sx,x0,xn);
	fprintf(filey,"%d %f %f\n",Sy,y0,xn);
	fprintf(filez,"%d %f %f\n",Sz,z0,xn);
	
	fprintf(filex,"%f %f\n",4.0,0);
	fprintf(filey,"%f %f\n",4.0,0);
	fprintf(filez,"%f %f\n",4.0,0);	
	
    double dt=0.000001;			//�������� dt
    memset(masprev, 0, Sx*Sy*Sz*sizeof(double));
	memset(masnext, 0, Sx*Sy*Sz*sizeof(double));
    for (int x=1; x<Sx-1; x++)
        for(int y=1; y<Sy-1; y++)
            for(int z=1; z<Sz-1; z++)
			{
                masprev[x+y*Sx+z*Sx*Sy]=u(x0+dx*x, y0+dy*y, z0+dz*z);
				if ((dx*x>=5) && (dx*x<=5.2) && (dy*y>=5) && (dy*y<=5.2) && (dz*z>=5) && (dz*z<=5.2))
					f[x+y*Sx+z*Sx*Sy]=4.0;
				else f[x+y*Sx+z*Sx*Sy]=0;
			}
	
	double Time;
	double _Time;
    double *dev_a,*dev_b,*dev_f;
	cudaMalloc( (void**)&dev_a, Sx*Sy*Sz*sizeof(double));
	cudaMalloc( (void**)&dev_b, Sx*Sy*Sz*sizeof(double));
	cudaMalloc( (void**)&dev_f, Sx*Sy*Sz*sizeof(double));

	cudaMemset(dev_a,0, Sx*Sy*Sz*sizeof(double));
	cudaMemset(dev_b,0, Sx*Sy*Sz*sizeof(double));
	cudaMemset(dev_f,0, Sx*Sy*Sz*sizeof(double));
	cudaMemcpy(dev_a,masprev,Sx*Sy*Sz*sizeof(double),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_f,f,Sx*Sy*Sz*sizeof(double),cudaMemcpyHostToDevice);

	double ddx=1.0/(dx*dx);
	double ddy=1.0/(dy*dy);
	double ddz=1.0/(dz*dz);

	Time=PortableGetTime();
	for (int t=1; t<St; t++)
    {
		StartCuda(dev_a,dev_b,Sx, Sy, Sz,dx,dy,dz,x0,y0,z0,dt,ddx,ddy,ddz,dev_f);
		
		double* tmp=dev_b;
		dev_b=dev_a;
		dev_a=tmp;
    }
	_Time=PortableGetTime();
	AllTime=_Time-Time;

	cudaMemcpy(masprev,dev_a,Sx*Sy*Sz*sizeof(double),cudaMemcpyDeviceToHost);

	for(int x=1; x<Sx-1; x++)
		fprintf(filex,"%lf ", masprev[x+49*Sx+49*Sx*Sy]);
	for(int y=1; y<Sy-1; y++)
		fprintf(filey,"%lf ", masprev[49+y*Sx+49*Sx*Sy]);
	for(int z=1; z<Sz-1; z++)
		fprintf(filez,"%lf ", masprev[49+49*Sx+z*Sx*Sy]);

	fprintf(filex,"\n");
	fprintf(filey,"\n");
	fprintf(filez,"\n");
	printf(" %lf \n",AllTime);
	system("PAUSE");
	fclose(filex);
    fclose(filey);
    fclose(filez);
	delete[] masprev;
	delete[] masnext;
	delete[] f;
	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_f);
    return 0;
}