#define minePI 3.14159265358979323846
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "crsMatrix.h"

int main ()
{
	const int Sx=100, Sy=100, Sz=100;
	const double dt=0.001;
	const int _k = 1;
	const int sigma = 1;
	double dx = 10.0/Sx, dy = 10.0/Sy, dz = 10.0/Sz;
	double *b = new double[(Sx-2)*(Sy-2)*(Sz-2)];	// вектор b
	double *f = new double[(Sx-2)*(Sy-2)*(Sz-2)];   // начальная функция
	double *x1 = new double[(Sx-2)*(Sy-2)*(Sz-2)];  // вектор решения 1 (предыдущий)
	double *x2 = new double[(Sx-2)*(Sy-2)*(Sz-2)];  // вектор решения 2 (следующий)
	memset(x1, 0, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));
	memset(x2, 0, (Sx-2)*(Sy-2)*(Sz-2)*sizeof(double));
	crsMatrix *A = new crsMatrix[1];
	
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

	A->setMatrix(Sx, Sy, Sz); 

	for(int i = 0; i < Sx-2; i++)
		for(int j = 0; j < Sy-2; j++)
			for(int k = 0; k < Sz-2; k++)
			{ 
				if (((i+1)*dx<5) && ((j+1)*dy<5) && ((k+1)*dz<5))
					x1[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=sin(minePI*(i+1)*dx/5)+sin(minePI*(j+1)*dy/5)+sin(minePI*(k+1)*dz/5); 
				else x1[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=0;
			
				if (((i+1)*dx >= 5) && ((i+1)*dx <= 5.2) && ((j+1)*dy>=5) && ((j+1)*dy <= 5.2) && ((k+1)*dz >= 5) && ((k+1)*dz <= 5.2))
					f[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=4.0;
				else f[i*(Sx-2)*(Sy-2)+j*(Sx-2)+k]=0;
			}
			for(int x=1; x<Sx-1; x++)
				fprintf(filex,"%lf ", x1[x+49*Sx+49*Sx*Sy]);
			for(int y=1; y<Sy-1; y++)
				fprintf(filey,"%lf ", x1[49+y*Sx+49*Sx*Sy]);
			for(int z=1; z<Sz-1; z++)
				fprintf(filez,"%lf ", x1[49+49*Sx+z*Sx*Sy]);

			fprintf(filex,"\n"); // переводим указатель на новую строчкку
			fprintf(filey,"\n"); // переводим указатель на новую строчкку
			fprintf(filez,"\n"); // переводим указатель на новую строчкку
				
	for (int i = 0; i<(Sx-2)*(Sy-2)*(Sz-2); i++)
    {
        b[i]=f[i]*dt+x1[i];
    }

	A->fillMatrix(sigma, _k, dt, dx, dy, dz, Sx, Sy, Sz);

	for (int time=0; time<100; time++)
	{
		int iteration = A->methodJacobi(x1,x2,b,1e-9);
		double* tmp = x1;
		x1 = x2;
		x2 = tmp;
		for (int i = 0; i<(Sx-2)*(Sy-2)*(Sz-2); i++)
		{
			b[i]=f[i]*dt+x1[i];
		}
		
			for(int x=1; x<Sx-1; x++)
				fprintf(filex,"%lf ", x1[x+49*Sx+49*Sx*Sy]);
			for(int y=1; y<Sy-1; y++)
				fprintf(filey,"%lf ", x1[49+y*Sx+49*Sx*Sy]);
			for(int z=1; z<Sz-1; z++)
				fprintf(filez,"%lf ", x1[49+49*Sx+z*Sx*Sy]);

			fprintf(filex,"\n"); // переводим указатель на новую строчкку
			fprintf(filey,"\n"); // переводим указатель на новую строчкку
			fprintf(filez,"\n"); // переводим указатель на новую строчкку
	}

}
