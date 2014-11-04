#include <stdio.h>
#include <string.h>
#include "portable_time.h"
#include <ctime>
#include <stdlib.h>
#include <cuda_runtime_api.h>
#include "kernel.cuh"


void StartCuda();

int main()
{

	//ввод данных 
		int n; // размер матрицы A и B
		int *A;
		int *B;
		int *C;
		FILE* output=fopen("output.txt","w+");  
		scanf("%d",&n);
		srand(time(0));
		A=new int[n*n];
		B=new int[n*n];
		for (int i=0;i<n;i++)
		{
			for (int j=0;j<n;j++)
			{
				A[i*n+j]=rand()% n;
				B[i*n+j]=rand()% n;
			}
		}
		C=new int[n*n];
		memset(C, 0, n*n*sizeof(int));
	//	рассчет влоб!
		double time=PortableGetTime();
		for(int i=0; i<n;i++)
			for(int j=0;j<n;j++)
			{
				for(int k=0; k<n; k++)
				{
					C[i*n+j]+=A[i*n+k]*B[k*n+j];
				}
					
			}
		double time1=PortableGetTime();
	// вывод в консоль
		for (int i=0;i<n;i++)
		{
			for (int j=0;j<n;j++)
			{
				fprintf(output,"%d ",C[i*n+j]);
			}
			fprintf(output,"\n");
		}
		fprintf(output,"%lf\n",time1-time);
		
	// Параллелим на cuda 
		int *dev_a,*dev_b,*dev_c;
		// выделяем память на gpu
		cudaMalloc( (void**)&dev_a, n*n*sizeof(int));
		cudaMalloc( (void**)&dev_b, n*n*sizeof(int));
		cudaMalloc( (void**)&dev_c, n*n*sizeof(int));
		//копируем массивы А и B на gpu
		cudaMemcpy(dev_a, A, n*n*sizeof(int),cudaMemcpyHostToDevice);
		cudaMemcpy(dev_b, B, n*n*sizeof(int),cudaMemcpyHostToDevice);
		cudaMemset(dev_c,0,n*n*sizeof(int));
		//запуск cuda
		time=PortableGetTime();

		StartCuda(dev_a,dev_b,dev_c,n);

		time1=PortableGetTime();
		//копируем обратно
		cudaMemcpy(C,dev_c,n*n*sizeof(int),cudaMemcpyDeviceToHost);

		for (int i=0;i<n;i++)
		{
			for (int j=0;j<n;j++)
			{
				fprintf(output,"%d ",C[i*n+j]);
			}
			fprintf(output,"\n");
		}
		fprintf(output,"%lf\n",time1-time);

		fclose(output);
		cudaFree(dev_a);
		cudaFree(dev_b);
		cudaFree(dev_c);
		delete A;
		delete B;
		delete C;
}
