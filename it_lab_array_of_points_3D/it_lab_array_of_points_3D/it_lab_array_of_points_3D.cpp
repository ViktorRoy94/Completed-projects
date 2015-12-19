// it_lab_array_of_points_3D.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include <string.h>
#include <omp.h>

#define minePI 3.14159265358979323846
int main(int argc, char* argv[])
{
	omp_set_dynamic(0);
	//omp_set_num_threads(4);
	printf("number of threads: %d/n",omp_get_max_threads());
	int n=100; 
	if (argc>1) n=atoi(argv[1]);
	const int g=1;
	const int _k=1;
	const double dt=0.000001;
	const int t=10;
	double* u=new double[n*n*n];// функция u(n)  
	double* un=new double[n*n*n]; // функция u(n+1)
	double* f=new double[n*n*n];
	double* tmp; // временная переменная для того, чтобы поменять u и un местами
	double up_xyz=10,down_xyz=0; // up_xyz,down_xyz - границы отрезка по x,y,z
	double h=(up_xyz-down_xyz)/n;
	double up_f=4,down_f=0; // граница значений по u ф-ции f
	double TIME;

	FILE* fpx=fopen("d:\\array_of_points_x.txt","w+");
	FILE* fpy=fopen("d:\\array_of_points_y.txt","w+");
	FILE* fpz=fopen("d:\\array_of_points_z.txt","w+");
	fprintf(fpx,"%d %f %f\n",n,down_xyz,up_xyz);
	fprintf(fpy,"%d %f %f\n",n,down_xyz,up_xyz);
	fprintf(fpz,"%d %f %f\n",n,down_xyz,up_xyz);
	
	fprintf(fpx,"%f %f\n",up_f,down_f);
	fprintf(fpy,"%f %f\n",up_f,down_f);
	fprintf(fpz,"%f %f\n",up_f,down_f);
	
	memset(u, 0, n*n*n*sizeof(double));
	memset(un, 0, n*n*n*sizeof(double));

	double time = PortableGetTime(); // time - узнаем пройденое время до начала рассчета
	
	for(int i=1;i<n-1;i++)
	{
		for(int j=1;j<n-1;j++)
		{
			for(int k=1;k<n-1;k++)
			{ 
		
				if ((i*h<5) && (j*h<5) && (k*h<5))
					u[i*n*n+j*n+k]=sin(minePI*i*h/5)+sin(minePI*j*h/5)+sin(minePI*k*h/5); // поправить 3.14. более точно
				else u[i*n*n+j*n+k]=0;
			
				if ((i*h>=5) && (i*h<=5.2) && (j*h>=5) && (j*h<=5.2) && (k*h>=5) && (k*h<=5.2))
					f[i*n*n+j*n+k]=up_f;
				else f[i*n*n+j*n+k]=0;
			}
		}
	}
	
	double _time=PortableGetTime(); // узнаем пройденное время после рассчета
	TIME =_time-time; // время затраченное только на рассчет

	time=PortableGetTime();		
	for(int Time=0;Time<t;Time++) // Time счетчик по времени
	{
		#pragma omp parallel for		
		for(int i=1;i<n-1;i++)
		{
			for(int j=1;j<n-1;j++)
			{
				for(int k=1;k<n-1;k++)
				{
					un[i*n*n+j*n+k]=dt*((u[(i+1)*n*n+j*n+k]-2*u[i*n*n+j*n+k]+u[(i-1)*n*n+j*n+k])/(h*h)+
					(u[i*n*n+(j+1)*n+k]-2*u[i*n*n+j*n+k]+u[i*n*n+(j-1)*n+k])/(h*h)+(u[i*n*n+j*n+(k+1)]-
					2*u[i*n*n+j*n+k]+u[i*n*n+j*n+(k-1)])/(h*h) - _k*u[i*n*n+j*n+k]+f[i*n*n+j*n+k])+
					u[i*n*n+j*n+k]; // находим u(n+1) 
				}
			}
		}
		tmp=u;   //
		u=un;	 // меняем u(n+1) и u(n) местами
		un=tmp;  //

		for(int i=1;i<n-1;i++)
			fprintf(fpx,"%lf ",un[i*n*n+49*n+49]);				
		for(int j=1;j<n-1;j++)
			fprintf(fpy,"%lf ",un[49*n*n+j*n+49]);				
		for(int k=1;k<n-1;k++)
			fprintf(fpz,"%lf ",un[49*n*n+49*n+k]);				
		fprintf(fpx,"\n"); // переводим указатель на новую строчкку
		fprintf(fpy,"\n"); // переводим указатель на новую строчкку
		fprintf(fpz,"\n"); // переводим указатель на новую строчкку
	}

	for(int i=1;i<n-1;i++)
		fprintf(fpx,"%lf ",un[i*n*n+49*n+49]);				
	for(int j=1;j<n-1;j++)
		fprintf(fpy,"%lf ",un[49*n*n+j*n+49]);				
	for(int k=1;k<n-1;k++)
		fprintf(fpz,"%lf ",un[49*n*n+49*n+k]);				
	fprintf(fpx,"\n"); // переводим указатель на новую строчкку
	fprintf(fpy,"\n"); // переводим указатель на новую строчкку
	fprintf(fpz,"\n"); // переводим указатель на новую строчкку

	_time=PortableGetTime();
	TIME+=(_time-time);

	fclose(fpx);
	fclose(fpy);
	fclose(fpz);
	printf("%lf\n",TIME);
	system("PAUSE");
	delete [] un;
	delete [] u;
	delete [] f;
	return 0;
}