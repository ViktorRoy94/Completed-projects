#pragma once
class crsMatrix
{
public:
	int N;		   // размер матрицы
	int NZ;		   // кол-во ненулевых элементов
	double *value; // массив значений (размер NZ)
	int *col;	   // массив номеров столбцов (размер NZ)
	int *rowIndex; // массив индексов строк (размер N+1)

public:
	crsMatrix()
	{
		N = 0;
		NZ = 0;
		value = 0;
		col = 0;
		rowIndex = 0;
	}
	int getNZ ()
	{
		return NZ;
	}
	int getN ()
	{
		return N;
	}
	double* Multiplicate(double *x)
	{
		double *b = new double[N];      // результат умножения матрицы на вектор
		for ( int i=0; i<N; i++)
		{
			b[i] = 0;
			for (int j=rowIndex[i]; j<rowIndex[i+1]; j++)
				b[i] +=value[j] * x[col[j]];
		}
		return b;
	}
	
	void setMatrix(unsigned int Sx, unsigned int Sy, unsigned int Sz)
	{
		Sx-=2;		// 
		Sy-=2;		// исключаем граничные точки из разностной сетки
		Sz-=2;		//

		N = Sx*Sy*Sz;
		rowIndex = new int[Sx*Sy*Sz+1];		// выделяем память для массива индексов начала строк матрицы
		int count = 1;	// cчетчик
		
		rowIndex[0] = 0;			// начнем заполнение массива индексов строк
		
		rowIndex[count++] = rowIndex[count-1] + 4;              // к-во соседей в начальной точке (угловая точка)
		for ( int i = 2; i<Sx; i++)									// 
		{															// к-во соседей у точек на грани Ox
			rowIndex[count++] = rowIndex[count-1] + 5;				//		
		}															//
		rowIndex[count++] = rowIndex[count-1] + 4;				// к-во соседей у угловой точки

		for ( int j = 2; j<Sy; j++)								// 
		{														//	
			rowIndex[count++] = rowIndex[count-1] + 5;			//
			for ( int i = 2; i<Sx; i++)							//
			{													//
				rowIndex[count++] = rowIndex[count-1] + 6;		//
			}													//
			rowIndex[count++] = rowIndex[count-1] + 5;			//
		}														//	к-во соседей у точек на нижней грани исключая ребро Ox	
																//	
		rowIndex[count++] = rowIndex[count-1] + 4;				//
		for ( int i = 2; i<Sx; i++)								//
		{														//
			rowIndex[count++] = rowIndex[count-1] + 5;			//			
		}														//
		rowIndex[count++] = rowIndex[count-1] + 4;				//


		for ( int k = 2; k<Sz; k++)									//
		{															//
			rowIndex[count++] = rowIndex[count-1] + 5;              //
			for ( int i = 2; i<Sx; i++)								//
			{														//
				rowIndex[count++] = rowIndex[count-1] + 6;			//	
			}														//					
			rowIndex[count++] = rowIndex[count-1] + 5;				// 
																	//			
			for ( int j = 2; j<Sy; j++)								// 
			{														//	
				rowIndex[count++] = rowIndex[count-1] + 6;			//
				for ( int i = 2; i<Sx; i++)							//
				{													//
					rowIndex[count++] = rowIndex[count-1] + 7;		//
				}													//
				rowIndex[count++] = rowIndex[count-1] + 6;			//
			}														//	к-во соседей у точек куба без верхней грани
																	//	
			rowIndex[count++] = rowIndex[count-1] + 5;				//
			for ( int i = 2; i<Sx; i++)								//
			{														//
				rowIndex[count++] = rowIndex[count-1] + 6;			//			
			}														//
			rowIndex[count++] = rowIndex[count-1] + 5;				//
		}

		rowIndex[count++] = rowIndex[count-1] + 4;              // 
		for ( int i = 2; i<Sx; i++)								// 
		{														//
			rowIndex[count++] = rowIndex[count-1] + 5;			//
		}														//	
		rowIndex[count++] = rowIndex[count-1] + 4;				//
																//
		for ( int j = 2; j<Sy; j++)								// 
		{														//	
			rowIndex[count++] = rowIndex[count-1] + 5;			//
			for ( int i = 2; i<Sx; i++)							//
			{													//
				rowIndex[count++] = rowIndex[count-1] + 6;		// к-во соседей у верхней грани
			}													//
			rowIndex[count++] = rowIndex[count-1] + 5;			//
		}														//
																//	
		rowIndex[count++] = rowIndex[count-1] + 4;				//
		for ( int i = 2; i<Sx; i++)								//
		{														//
			rowIndex[count++] = rowIndex[count-1] + 5;			//			
		}														//
		rowIndex[count] = rowIndex[count-1] + 4;				// закончили заполнять массив индексов строк
		NZ = rowIndex[count];
		col = new int[NZ];				// выделяем память для массива столбцов матрицы
		value = new double[NZ];			// выделяем память для массива значений матрицы

		count = 0;

		for( int i=0; i<N; i++)       // начинаем заполнять массив значений номеров столбцов
		{
			if(i/(Sx*Sy))
			{
				col[count++]=i-Sx*Sy;				// есть сосед снизу
			}
			if((i/(Sx))%Sy)
			{
				col[count++]=i-Sx;					// есть сосед слева
			}
			if(i%Sx)
			{
				col[count++]=i-1;					// есть сосед спереди
			}

			col[count++]=i;							// сама точка

			if(i%Sx!=(Sx-1))
			{
				col[count++]=i+1;					// есть сосед сзади
			}
			if((i/(Sx)%Sy)!=(Sy-1))
			{
				col[count++]=i+Sx;					// есть сосед справа
			}
			if(i/(Sx*Sy)!=(Sz-1))			
			{
				col[count++]=i+Sx*Sy;				// есть сосед сверху
			}
		}
	}

	void fillMatrix (int sigma, int _k, double dt, double dx, double dy, double dz, int Sx, int Sy, int Sz)
	{
		Sx -= 2; 
		Sy -= 2; 
		Sz -= 2; 
		int count = 0;				 // счетчик
		for( int i=0; i<N; i++)      // заполняем матрицу коэф-ми
		{
			if(i/(Sx*Sy))
			{
				value[count++] = -sigma*dt/(dz*dz);				// есть сосед снизу
			}
			if((i/(Sx))%Sy)
			{
				value[count++] = -sigma*dt/(dy*dy);				// есть сосед слева
			}
			if(i%Sx)
			{
				value[count++] = -sigma*dt/(dx*dx);				// есть сосед спереди
			}

			value[count++] = 1 + _k*dt + 2*sigma*dt*(1/(dx*dx) + 1/(dy*dy) + 1/(dz*dz));		// сама точка

			if(i%Sx!=(Sx-1))
			{
				value[count++] = -sigma*dt/(dx*dx);				// есть сосед сзади
			}
			if((i/(Sx)%Sy)!=(Sy-1))
			{
				value[count++] = -sigma*dt/(dy*dy);				// есть сосед справа
			}
			if(i/(Sx*Sy)!=(Sz-1))
			{
				value[count++] = -sigma*dt/(dz*dz);				// есть сосед сверху
			}
		}
	}

	~crsMatrix()
	{
		delete [] value;
		delete [] col;
		delete [] rowIndex;
	}

};


