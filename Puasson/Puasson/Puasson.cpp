#include <stdio.h> 
#include "portable_time.h"
#include <string>
#include <omp.h>
void fillBorders (const int* start, int countStart, int* finish, int countFinish); // функция для маштабирования данных начальных условий

void main () 
{
		int *kgrl, *kgrp, *kgrv, *kgrn;		// массивы отмаштабируемых граничных условий
		int iterationCount = 10000;			// общее кол-во итераций
		double Flop = 0;					// к-во операций с плавующей точкой
		double sizeX, sizeY, sizeZ;			// размер выделенной области
		int Nx, Ny;							// к-во точек разбиения 
		double x, y;						// шаг по x и y соответственно
		double x2, y2;						// квадраты шага по времени
		double *v;							// массив итоговых значений
		double lambda;						// коэф-т теплопроводности Вт/мкм
		double om;							// вспомогательный коэф-т
		int BCTCount = 4;					// к-во условий по температуре
		int BCWCount = 4;					// к-во условий по напряжению
		double *BCT, *BCW;					// массивы условий по температуре и напряжению
		double Time;						// итоговое время работы
		int BCCountTop;						// 
		int BCCountBottom;					// размеры массивов начальных условий,
		int BCCountLeft;					// данных изначально
		int BCCountRight;					//
		int *BCTop;							// массивы граничных условий,
		int *BCBottom;						// данные изначально
		int *BCLeft;						//
		int *BCRight;						//

		double *BCWFlagTop;						//
		double *BCWFlagBottom;					//
		double *BCWFlagLeft;					//
		double *BCWFlagRight;					//

		double *BCWTop;							//
		double *BCWBottom;						//
		double *BCWLeft;						//
		double *BCWRight;						//

		double *BCTTop;							//
		double *BCTBottom;						//
		double *BCTLeft;						//
		double *BCTRight;						//

		FILE* fp=fopen("input.txt","r");
		FILE* fo=fopen("output.txt","w");

		fscanf(fp, "%lf\n", &sizeX);
		fscanf(fp, "%lf\n", &sizeY);
		fscanf(fp, "%lf\n", &sizeZ);
		fscanf(fp, "%d \n", &Nx);
		fscanf(fp, "%d \n", &Ny);	
		fscanf(fp, "%lf\n", &lambda);
		fscanf(fp, "%d \n", &BCCountTop);
		fscanf(fp, "%d \n", &BCCountBottom);
		fscanf(fp, "%d \n", &BCCountLeft);
		fscanf(fp, "%d \n", &BCCountRight);
		
		kgrl = new int[Ny];
		kgrp = new int[Ny];
		kgrn = new int[Nx];
		kgrv = new int[Nx];
		BCT  = new double[BCTCount+1];
		BCW  = new double[BCWCount+1];
		v	 = new double[Nx*Ny];
		BCTop =    new int[BCCountTop];
		BCBottom = new int[BCCountBottom];
		BCLeft =   new int[BCCountLeft];
		BCRight =  new int[BCCountRight];
		
		BCWTop =    new double[Nx];
		BCWBottom = new double[Nx];
		BCWLeft =   new double[Ny];
		BCWRight =  new double[Ny];

		BCTTop =    new double[Nx];
		BCTBottom = new double[Nx];
		BCTLeft =   new double[Ny];
		BCTRight =  new double[Ny];
		
		BCWFlagTop =    new double[Nx];
		BCWFlagBottom = new double[Nx];
		BCWFlagLeft =   new double[Ny];
		BCWFlagRight =  new double[Ny];
		
		memset(BCWFlagTop   , 0, Nx*sizeof(double));
		memset(BCWFlagBottom, 0, Nx*sizeof(double));
		memset(BCWFlagLeft  , 0, Ny*sizeof(double));
		memset(BCWFlagRight , 0, Ny*sizeof(double));

		memset(BCWTop   , 0, Nx*sizeof(double));
		memset(BCWBottom, 0, Nx*sizeof(double));
		memset(BCWLeft  , 0, Ny*sizeof(double));
		memset(BCWRight , 0, Ny*sizeof(double));

		memset(BCTTop   , 0, Nx*sizeof(double));
		memset(BCTBottom, 0, Nx*sizeof(double));
		memset(BCTLeft  , 0, Ny*sizeof(double));
		memset(BCTRight , 0, Ny*sizeof(double));



		x = sizeX/Nx;				// если шаг по времени то dx? 
		y = sizeY/Ny;

		x2 = x * x;
		y2 = y * y;
		
		for (int i=0; i<BCCountTop; i++)
		{
			fscanf(fp, "%d", &BCTop[i]);
		}
		fscanf(fp,"\n");
		
		for (int i=0; i<BCCountBottom; i++)
		{
			fscanf(fp, "%d", &BCBottom[i]);
		}
		fscanf(fp,"\n");
		
		for (int i = 0; i < BCCountLeft; i++)
		{	
			fscanf(fp, "%d", &BCLeft[i]);
		}
		fscanf(fp, "\n");
		
		for (int i = 0; i < BCCountRight; i++)
		{	
			fscanf(fp, "%d", &BCRight[i]);
		}
		fscanf(fp, "\n");

		fillBorders(BCTop,   BCCountTop,   kgrv,Nx);
		fillBorders(BCBottom,BCCountBottom,kgrn,Nx);
		fillBorders(BCRight, BCCountRight, kgrp,Ny);
		fillBorders(BCLeft,  BCCountLeft,  kgrl,Ny);

		om = 0.9 * x2 * y2 / (x2 + y2);
		
		BCT[0] = 0;
		for (int i = 1; i < BCTCount + 1; i++)
		{	
			fscanf(fp, "%lf", &BCT[i]);
		}
		fscanf(fp, "\n");
		
		BCW[0] = 0;	
		for (int i = 1; i < BCWCount + 1; i++)
		{	
			fscanf(fp, "%lf", &BCW[i]);
		}
		fscanf(fp, "\n");		
		
		for (int i = 0; i < Ny; i++)
		{
			for (int j = 0; j < Nx; j++)
			{
				v[i*Nx+j] = 0;
			}
		}

		for (int i = 0; i < Nx; i++)
		{
			if (kgrv[i] <= 0) 
			{
				BCWTop[i] = BCW [-kgrv[i]];
				BCWFlagTop[i] = 1.0;
			}
			if (kgrv[i] >= 0) BCTTop[i] = BCT[kgrv[i]];
		}
		for (int i = 0; i < Nx; i++)
		{
			if (kgrn[i] <= 0) 
			{
				BCWBottom[i] = BCW [-kgrn[i]];
				BCWFlagBottom[i] = 1.0;
			}
			if (kgrn[i] >= 0) BCTBottom[i] = BCT[kgrn[i]];
		}
		for (int i = 0; i < Ny; i++)
		{
			if (kgrl[i] <= 0) 
			{
				BCWLeft[i] = BCW [-kgrl[i]];
				BCWFlagLeft[i] = 1.0;
			}
			if (kgrl[i] >= 0) BCTLeft[i] = BCT[kgrl[i]];
		}
		for (int i = 0; i < Ny; i++)
		{
			if (kgrp[i] <= 0) 
			{
				BCWRight[i] = BCW [-kgrp[i]];
				BCWFlagRight[i] = 1;
			}
			if (kgrp[i] >= 0) BCTRight[i] = BCT[kgrp[i]];
		}

		for (int i = 0; i < Nx; i++)
		{
			if (kgrv[i] > 0) v[i          ] = BCT[kgrv[i]];
			if (kgrn[i] > 0) v[Nx*(Ny-1)+i] = BCT[kgrn[i]];
		}

		for (int i = 0; i < Ny; i++)
		{
			if (kgrl[i] > 0) v[i*Nx     ] = BCT[kgrl[i]];
			if (kgrp[i] > 0) v[i*Nx+Nx-1] = BCT[kgrp[i]];
		}

		Time = PortableGetTime();
		//for (int l = 0; l < iterationCount; l++)  // цикл моделирования 
		//{
		//	for (int i = 1; i < Ny-1; i++)
		//	{
		//		for (int j = 1; j < Nx-1; j++)
		//		{
		//			v[i*Nx+j] = -0.8 * v[i*Nx+j] + om * (( v[(i+1)*Nx+j] + v[(i-1)*Nx+j]) / y2 + (v[i*Nx+j+1] + v[i*Nx+j-1]) / x2);
		//			Flop = Flop + 7;
		//		}
		//	}
		//	
		//	double t = y / 	(lambda * sizeZ * x);   // вспомогательный коэф-т
		//	#pragma omp parallel for 
		//	for (int j = 0; j < Nx; j++)
		//	{
		//		v[j          ] = BCTTop[j]    + BCWFlagTop[j]    * (v[Nx+j       ] + (BCWTop[j]    * t));
		//		v[Nx*(Ny-1)+j] = BCTBottom[j] + BCWFlagBottom[j] * (v[Nx*(Ny-2)+j] + (BCWBottom[j] * t));
		//	}

		//	t = x / (lambda * sizeZ * y);
		//	for (int i = 1; i < Ny-1; i++)
		//	{
		//		v[i*Nx     ] = BCTLeft[i]  + BCWFlagLeft [i] * (v[i*Nx+1   ] + (BCWLeft[i]  * t));
		//		v[i*Nx+Nx-1] = BCTRight[i] + BCWFlagRight[i] * (v[i*Nx+Nx-2] + (BCWRight[i] * t));
		//	}

		//	//if (l % 100 == 0)
		//	//{	
		//	//	fprintf(fo,"----------------------------------------------\n");
		//	//	for (int i = 0; i < Ny; i++)
		//	//	{
		//	//		for (int j = 0; j < Nx; j++)
		//	//		{
		//	//			fprintf(fo,"%4.2lf ", v[i*Nx+j]);
		//	//		}
		//	//		fprintf(fo,"\n");
		//	//	}
		//	//}
		//}
		
		int start;								// границы области для каждого потока
		int end;
		omp_set_num_threads(4);		
		int MT = omp_get_max_threads();
		int chunk  = (Ny-2 + MT-1)/MT;						// размер области для каждого потока 
		for (int t = MT-1; t>=1; t--)				// начало метода волны
		{
			start = 1;							
			end = chunk * t;				
			for (int i = start; i <= end; i++)
				for (int j = 1; j < Nx-1; j++)
				{
					v[i*Nx+j] = -0.8 * v[i*Nx+j] + om * (( v[(i+1)*Nx+j] + v[(i-1)*Nx+j]) / y2 + (v[i*Nx+j+1] + v[i*Nx+j-1]) / x2);
					Flop = Flop + 7;
				}
				double t = y / 	(lambda * sizeZ * x);   // вспомогательный коэф-т
			
				for (int j = 0; j < Nx; j++)			// надо ли от 1 до N-1?? 
				{
					v[j          ] = BCTTop[j]    + BCWFlagTop[j]    * (v[Nx+j       ] + (BCWTop[j]    * t));
				}
			
				t = x / (lambda * sizeZ * y);
			
				for (int i = 1; i <= end; i++)
				{
					v[i*Nx     ] = BCTLeft[i]  + BCWFlagLeft [i] * (v[i*Nx+1   ] + (BCWLeft[i]  * t));
					v[i*Nx+Nx-1] = BCTRight[i] + BCWFlagRight[i] * (v[i*Nx+Nx-2] + (BCWRight[i] * t));
				}
		}


		for (int l = 0; l < iterationCount - MT+1; l++)  // цикл моделирования 
		{
			#pragma omp parallel private (start, end)
			{
				start = 1 + omp_get_thread_num()*chunk;
				end = start + chunk - 1;
				if (end > Ny-2)  end = Ny - 2;
				for (int i = start; i < end; i++)
				{
					for (int j = 1; j < Nx-1; j++)
					{
						v[i*Nx+j] = -0.8 * v[i*Nx+j] + om * (( v[(i+1)*Nx+j] + v[(i-1)*Nx+j]) / y2 + (v[i*Nx+j+1] + v[i*Nx+j-1]) / x2);
						Flop = Flop + 7;
					}
				}
				#pragma omp barrier
				for (int i = end; i <= end; i++)
				{
					for (int j = 1; j < Nx-1; j++)
					{
						v[i*Nx+j] = -0.8 * v[i*Nx+j] + om * (( v[(i+1)*Nx+j] + v[(i-1)*Nx+j]) / y2 + (v[i*Nx+j+1] + v[i*Nx+j-1]) / x2);
						Flop = Flop + 7;
					}
				}
				#pragma omp barrier
			}
			double t = y / 	(lambda * sizeZ * x);   // вспомогательный коэф-т
			
			#pragma parallel for
			for (int j = 0; j < Nx; j++)
			{
				v[j          ] = BCTTop[j]    + BCWFlagTop[j]    * (v[Nx+j       ] + (BCWTop[j]    * t));
				v[Nx*(Ny-1)+j] = BCTBottom[j] + BCWFlagBottom[j] * (v[Nx*(Ny-2)+j] + (BCWBottom[j] * t));
			}
			
			t = x / (lambda * sizeZ * y);
			
			#pragma parallel for
			for (int i = 1; i < Ny-1; i++)
			{
				v[i*Nx     ] = BCTLeft[i]  + BCWFlagLeft [i] * (v[i*Nx+1   ] + (BCWLeft[i]  * t));
				v[i*Nx+Nx-1] = BCTRight[i] + BCWFlagRight[i] * (v[i*Nx+Nx-2] + (BCWRight[i] * t));
			}
		}

		for (int t = MT-1; t>=1; t--)				// конец метода волны
		{
			start = 1 + chunk * t;							
			end = Ny-2;				
			for (int i = start; i <= end; i++)
				for (int j = 1; j < Nx-1; j++)
				{
					v[i*Nx+j] = -0.8 * v[i*Nx+j] + om * (( v[(i+1)*Nx+j] + v[(i-1)*Nx+j]) / y2 + (v[i*Nx+j+1] + v[i*Nx+j-1]) / x2);
					Flop = Flop + 7;
				}
				double t = y / 	(lambda * sizeZ * x);   // вспомогательный коэф-т
			
				for (int j = 0; j < Nx; j++)			
				{
					v[Nx*(Ny-1)+j] = BCTBottom[j] + BCWFlagBottom[j] * (v[Nx*(Ny-2)+j] + (BCWBottom[j] * t));
				}
			
				t = x / (lambda * sizeZ * y);
			
				for (int i = start; i <= end; i++)
				{
					v[i*Nx     ] = BCTLeft[i]  + BCWFlagLeft [i] * (v[i*Nx+1   ] + (BCWLeft[i]  * t));
					v[i*Nx+Nx-1] = BCTRight[i] + BCWFlagRight[i] * (v[i*Nx+Nx-2] + (BCWRight[i] * t));
				}
		}

		Time = PortableGetTime() - Time;
		
		fprintf(fo,"Iterations = %d\n",iterationCount);
		fprintf(fo,"Time  = %lf\n", Time);
		fprintf(fo,"Flop  = %4.2lf\n",Flop);
		fprintf(fo,"Flops = %4.2lf\n", Flop/(Time*1000000000));

		fprintf(fo,"----------------------------------------------\n");
		for (int i = 0; i < Ny; i++)
		{
			for (int j = 0; j < Nx; j++)
			{
				fprintf(fo,"%4.2lf ", v[i*Nx+j]);
			}
			fprintf(fo,"\n");
		}

	fclose(fp);
	fclose(fo);
	delete [] BCWFlagRight;
	delete [] BCWFlagLeft;
	delete	[] BCWFlagBottom;
	delete	[] BCWFlagTop;

	delete	[] BCTRight;
	delete	[] BCTLeft;
	delete	[] BCTBottom;
	delete	[] BCTTop;

	delete	[] BCWRight;
	delete	[] BCWLeft;
	delete	[] BCWBottom;
	delete	[] BCWTop;

	delete [] BCRight;
	delete [] BCLeft;
	delete [] BCBottom;
	delete [] BCTop;

	delete [] v;
	delete [] BCT;
	delete [] BCW;

	delete [] kgrv;
	delete [] kgrn;
	delete [] kgrl;
	delete [] kgrp;

}		 

void fillBorders (const int* start, int countStart, int* finish, int countFinish)
{
	int i = 0, j = 0;
	
	while ( i < countFinish)
	{
		if (i * countStart > (j+1) * (countFinish - 1)) 
		{
			j++;
		}
		finish[i] = start [j];
		i++;
	}
}