//////////////////////////////////////////////////////////////////////////////////////////
//	Лабораторная работа: Численное решение ду параболического типа с использованием GPU //
//	Автор:				 Рой В.Д. (группа 8210)										    //
//	Дата:				 17.05.2014													    //
//--------------------------------------------------------------------------------------//
//  mainIDS.cpp -> файл-ядро программы												    //
//////////////////////////////////////////////////////////////////////////////////////////

#define minePI 3.14159265358979323846
#include <math.h>
#include <string.h>
#include <stdio.h> 
#include "crsMatrix.h"
#include <cuda_runtime_api.h>
#include "CUDA.cuh"
#include "portable_time.h"

int main ()
{
	const int Sx=300, Sy=300, Sz=300;
	const double dt=0.000001;	
	const int _k = 1;
	const int sigma = 1;
	double dx = 10.0/Sx, dy = 10.0/Sy, dz = 10.0/Sz;
	const double diag = 1 + _k*dt + 2*sigma*dt*(1/(dx*dx) + 1/(dy*dy) + 1/(dz*dz)); // элементы на диагонали матрицы. вводим с самого начала, чтобы потом не считать.
	double *b = new double[(Sx-2)*(Sy-2)*(Sz-2)];		// вектор b
	double *f = new double[(Sx-2)*(Sy-2)*(Sz-2)];		// начальная функция
	double *x1 = new double[(Sx-2)*(Sy-2)*(Sz-2)];		// вектор решения 1 (предыдущий)
	double *x2 = new double[(Sx-2)*(Sy-2)*(Sz-2)];		// вектор решения 2 (следующий)
	memset(x1, 0, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));
	memset(x2, 0, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));
	
	crsMatrix A;
	
	FILE* filex=fopen("filexIDS.txt","w");		//	файл с рез-ми по x
    FILE* filey=fopen("fileyIDS.txt","w");		//	файл с рез-ми по y
    FILE* filez=fopen("filezIDS.txt","w");		//	файл с рез-ми по z

	fprintf(filex,"%d %f %f\n",Sx,0.0,10.0);    //
	fprintf(filey,"%d %f %f\n",Sy,0.0,10.0);	//
	fprintf(filez,"%d %f %f\n",Sz,0.0,10.0);	// 
												//  запись информации о размере сетки
	fprintf(filex,"%f %f\n",4.0,0.0);			//
	fprintf(filey,"%f %f\n",4.0,0.0);			//
	fprintf(filez,"%f %f\n",4.0,0.0);			//

	A.setMatrix(Sx, Sy, Sz);					// создание портрета матрицы

	// задание начальных условий
	for(int i = 0; i < Sx-2; i++)				
		for(int j = 0; j < Sy-2; j++)
			for(int k = 0; k < Sz-2; k++)
			{ 
				if (((i+1)*dx<5) && ((j+1)*dy<5) && ((k+1)*dz<5)){
					x1[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=sin(minePI*(i+1)*dx/5)+sin(minePI*(j+1)*dy/5)+sin(minePI*(k+1)*dz/5); 
				}
				else x1[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=0;

				if (((i+1)*dx >= 5) && ((i+1)*dx <= 5.2) && ((j+1)*dy>=5) && ((j+1)*dy <= 5.2) && ((k+1)*dz >= 5) && ((k+1)*dz <= 5.2))
					f[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=4.0;
				else f[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=0;
			}
	// конец вывода начальных условий
			for(int x=0; x<Sx-2; x++)										//
				fprintf(filex,"%lf ", x1[x+24*(Sx-2)+24*(Sx-2)*(Sy-2)]);	//
			for(int y=0; y<Sy-2; y++)										//
				fprintf(filey,"%lf ", x1[24+y*(Sx-2)+24*(Sx-2)*(Sy-2)]);	// вывод в файл начальных условий
			for(int z=0; z<Sz-2; z++)										//
				fprintf(filez,"%lf ", x1[24+24*(Sx-2)+z*(Sx-2)*(Sy-2)]);	//

			fprintf(filex,"\n");											// 
			fprintf(filey,"\n");											// перевод указателя на новую строку в каждом файле
			fprintf(filez,"\n");											// 
				
	for (int i = 0; i<(Sx-2)*(Sy-2)*(Sz-2); i++)							//
    {																		// заполнение вектора b
        b[i]=f[i]*dt+x1[i];													//
    }																		//

	A.fillMatrix(sigma, _k, dt, dx, dy, dz, Sx, Sy, Sz);					// заполнение матрицы коэфициентами

	double *dev_x1,*dev_x2,*dev_value, *dev_b, *d_x1, *d_x2, *d_tmp, *dev_f;	// переменные для выделения памяти на gpu
	int *dev_col, *dev_rowIndex, *dev_iteration, iteration = 0;					// передаются массивы матрицы, вектора x1,x2,b
	double TIME, ALLTIME;												// переменные для определения время работы

	cudaMalloc( (void**)&dev_x1, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));				//
	cudaMalloc( (void**)&dev_x2, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));				//
	cudaMalloc( (void**)&dev_b, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));				// выделение памяти массивам dev_x1, 
	cudaMalloc( (void**)&dev_value, A.NZ*sizeof(double));							// dev_x2, dev_b, dev_value, dev_col,
	cudaMalloc( (void**)&dev_col, A.NZ*sizeof(int));								// dev_rowIndex, dev_iteration
	cudaMalloc( (void**)&dev_rowIndex, ((Sx-2)*(Sy-2)*(Sz-2)+1)*sizeof(int));		// на gpu
	cudaMalloc( (void**)&dev_iteration, sizeof(int));								//
	cudaMalloc( (void**)&dev_f, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));				//

	cudaMemcpy(dev_x1,x1,(Sx-2)*(Sy-2)*(Sz-2)*sizeof(double),cudaMemcpyHostToDevice);					//
	cudaMemcpy(dev_value,A.value,A.NZ*sizeof(double),cudaMemcpyHostToDevice);							//
	cudaMemcpy(dev_col,A.col,A.NZ*sizeof(int),cudaMemcpyHostToDevice);									//
	cudaMemcpy(dev_rowIndex,A.rowIndex,((Sx-2)*(Sy-2)*(Sz-2)+1)*sizeof(int),cudaMemcpyHostToDevice);	// копирование значений массивов
	cudaMemcpy(dev_iteration,&iteration,sizeof(int),cudaMemcpyHostToDevice);							// с начальными условиями 
	cudaMemset(dev_x2,0,(Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));											// в массивы на gpu
	cudaMemcpy(dev_b,b,(Sx-2)*(Sy-2)*(Sz-2)*sizeof(double),cudaMemcpyHostToDevice);						//
	cudaMemcpy(dev_f,f,(Sx-2)*(Sy-2)*(Sz-2)*sizeof(double),cudaMemcpyHostToDevice);						//
	d_x1 = dev_x1;					// промежуточные переменные созданные для определения 
	d_x2 = dev_x2;					// в каком массиве dev_x1 или dev_x2 находится решение на gpu
	
	TIME = PortableGetTime();		// замер времени перед функцией
	//int countIteration;
	for (int time=0; time<100; time++)		// цикл решения ду по времени
	{
		// запуск ядра на gpu
		StartCuda (dev_value, dev_col, dev_rowIndex,d_x1, d_x2, dev_b, 1e-9, 1 + _k*dt + 2*sigma*dt*(1/(dx*dx) + 1/(dy*dy) + 1/(dz*dz)),dev_iteration, Sx, Sy, Sz, dt,dev_f);
																								
		//cudaMemcpy(&iteration,dev_iteration,sizeof(int),cudaMemcpyDeviceToHost);
		//if( iteration % 2 ){
		//	d_tmp = d_x1;
		//	d_x1 = d_x2;
		//	d_x2 = d_tmp;
		//}
		//for (int i = 0; i<(Sx-2)*(Sy-2)*(Sz-2); i++)
		//{
		//	b[i]=f[i]*dt+x1[i];
		//}
	//	countIteration  += iteration;
	}
	ALLTIME = PortableGetTime() - TIME;			// рассчет времени
	printf(" %lf \n",ALLTIME);					// вывод времени
//	printf(" %d \n",countIteration);
	cudaMemcpy(x1,d_x1,(Sx-2)*(Sy-2)*(Sz-2)*sizeof(double),cudaMemcpyDeviceToHost);  // копирование с gpu решение
	for(int x=0; x<Sx-2; x++)										//
		fprintf(filex,"%lf ", x1[x+24*(Sx-2)+24*(Sx-2)*(Sy-2)]);	//
	for(int y=0; y<Sy-2; y++)										//
		fprintf(filey,"%lf ", x1[24+y*(Sx-2)+24*(Sx-2)*(Sy-2)]);	// вывод в файл решение ду
	for(int z=0; z<Sz-2; z++)										//
		fprintf(filez,"%lf ", x1[24+24*(Sx-2)+z*(Sx-2)*(Sy-2)]);	//
	fprintf(filex,"\n");											// 
	fprintf(filey,"\n");											// перевод указателя на новую строку в каждом файле
	fprintf(filez,"\n");											// 
	
	fclose(filex);				//
    fclose(filey);				// закрытие открытых файлов
    fclose(filez);				//

	cudaFree(dev_x1);			// 
	cudaFree(dev_x2);			//
	cudaFree(dev_b);			//
	cudaFree(dev_value);		//
	cudaFree(dev_rowIndex);		//
	cudaFree(dev_col);			// освобождение выделенной памяти на gpu
	cudaFree(dev_iteration);	//
	cudaFree(d_x1);				//
	cudaFree(d_x2);				//
	cudaFree(dev_f);			//
	delete [] x1;			//
	delete [] x2;			//
	delete [] b;			// освобождение файлов на cpu
	delete [] f;			//

}
