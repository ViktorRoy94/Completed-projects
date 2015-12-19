// It_lab_array of_points.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"

int main(void)
{
	const int n=10; 
	const int g=1;
	const int k=1;
	const double dt=0.001;
	const int t=100;
	double* u=new double[n];// функция u(n)  
	double* un=new double[n]; // функция u(n+1)
	double* x=new double[n];
	double* f=new double[n];
	double* tmp; // временная переменная
	double b=10,a=0; // b,a - границы отрезка
	double h=(b-a)/n;
	double y2=2,y1=0; // граница значений по y ф-ции f
	double time;

	FILE* fp1=fopen("array_of_points.txt","w");
	/*fprintf(fp1,"%d %f %f\n",t,a,b);
	fprintf(fp1,"%f %f\n",y2,y1);*/
	u[0]=0; 
	u[n-1]=0;	
	for (int i=1;i<n-1;i++) // начальные условия
	{
		x[i]=i*(b-a)/n;  // шаг клетки
		if (x[i]<5)     
			u[i]=sin(3.14*x[i]/5); // заполнение массива значениями фун-ции в начальный момент
		else 
			u[i]=0;
		
		if ((x[i]>=5) && (x[i]<=5.2))
			f[i]=2;
		else f[i]=0;

//		fprintf(fp1,"%f ",u[i]); // записываем u(0) в файл
		//fprintf(fp1,"%f ",x[i]); // записываем u(0) в файл
	}
	//fprintf(fp1,"\n");
	for (int i=1;i<n-1;i++){ // начальные условия
		fprintf(fp1,"%f ",u[i]); // записываем u(0) в файл
	}

	fprintf(fp1,"\n"); // переводим указатель на новую строчкку
	time=PortableGetTime();
	for (int j=0;j<t;j++)
	{
		un[0]=0;
		un[n-1]=0;
		for (int i=1;i<n-1;i++)
		{
			un[i]=dt*((u[i+1]-2*u[i]+u[i-1])/(h*h) - k*u[i]+f[i])+u[i]; // находим u(n+1) 
			fprintf(fp1,"%f ",un[i]);
		}
		tmp=u;   //
		u=un;	 // меняем u(n+1) и u(n) местам и теперь на основе un считаем u
		un=tmp;  //
		fprintf(fp1,"\n"); // переводим указатель на новую строчкку
	}
	fclose(fp1);
	return 0;
}




