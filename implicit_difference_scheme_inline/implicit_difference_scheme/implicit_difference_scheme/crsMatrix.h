class crsMatrix
{
private:
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

	double* Multiplicate(double *x)
	{
		double *b = new double[N];      // результат умножени€ матрицы на вектор
		for ( int i=0; i<N; i++)
		{
			b[i] = 0;
			for (int j=rowIndex[i]; j<rowIndex[i+1]; j++)
				b[i] +=value[j] * x[col[j]];
		}
		return b;
	}
	
/*	void printMatrix ()
	{
		for ( int i=0; i<N; i++)
		{
			for (int j=rowIndex[i]; j<rowIndex[i+1]; j++)
				b[i] +=value[j] * x[col[j]];
		}
	}*/

	void setMatrix(unsigned int Sx, unsigned int Sy, unsigned int Sz)
	{
		Sx-=2;
		Sy-=2;
		Sz-=2;

		N = Sx*Sy*Sz;
		rowIndex = new int[Sx*Sy*Sz+1];
		int count = 1;	// cчетчик
		
		rowIndex[0] = 0;			// начнем заполнение массива индексов строк
		
		rowIndex[count++] = rowIndex[count-1] + 4;              // к-во соседей в начальной точке (углова€ точка)
		for ( int i = 2; i<Sx; i++)								// к-во соседей у точек на грани Ox
		{
			rowIndex[count++] = rowIndex[count-1] + 5;			
		}
		rowIndex[count++] = rowIndex[count-1] + 4;				// к-во соседей у угловой точки

		for ( int j = 2; j<Sy; j++)								// 
		{														//	
			rowIndex[count++] = rowIndex[count-1] + 5;			//
			for ( int i = 2; i<Sx; i++)							//
			{													//
				rowIndex[count++] = rowIndex[count-1] + 6;		//
			}													//
			rowIndex[count++] = rowIndex[count-1] + 5;			//
		}														//	к-во соседей у точек на нижней грани исключа€ ребра Ox	
																//	
		rowIndex[count++] = rowIndex[count-1] + 4;				//
		for ( int i = 2; i<Sx; i++)								//
		{														//
			rowIndex[count++] = rowIndex[count-1] + 5;			//			
		}														//
		rowIndex[count++] = rowIndex[count-1] + 4;				//


		for ( int k = 2; k<Sz; k++)
		{
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
		rowIndex[count] = rowIndex[count-1] + 4;				// закончили заполн€ть массив индексов строк
		NZ = rowIndex[count];
		col = new int[NZ];
		value = new double[NZ];

		count = 0;

		for( int i=0; i<N; i++)       // начинаем заполн€ть массив значений номеров столбцов
		{
			if(i/(Sx*Sy))
			{
				col[count++]=i-Sx*Sy;
			}
			if((i/(Sx))%Sy)
			{
				col[count++]=i-Sx;
			}
			if(i%Sx)
			{
				col[count++]=i-1;
			}

			col[count++]=i;

			if(i%Sx!=(Sx-1))
			{
				col[count++]=i+1;
			}
			if((i/(Sx)%Sy)!=(Sy-1))
			{
				col[count++]=i+Sx;
			}
			if(i/(Sx*Sy)!=(Sz-1))
			{
				col[count++]=i+Sx*Sy;
			}
		}
	}

	void fillMatrix (int sigma, int _k, double dt, double dx, double dy, double dz, int Sx, int Sy, int Sz)
	{
		Sx -= 2; 
		Sy -= 2; 
		Sz -= 2; 
		int count = 0; // счетчик
		for( int i=0; i<N; i++)       // заполн€ем матрицу коэф-ми
		{
			if(i/(Sx*Sy))
			{
				value[count++] = -sigma*dt/(dz*dz);
			}
			if((i/(Sx))%Sy)
			{
				value[count++] = -sigma*dt/(dy*dy);
			}
			if(i%Sx)
			{
				value[count++] = -sigma*dt/(dx*dx);
			}

			value[count++] = 1 + _k*dt + 2*sigma*dt*(1/(dx*dx) + 1/(dy*dy) + 1/(dz*dz));

			if(i%Sx!=(Sx-1))
			{
				value[count++] = -sigma*dt/(dx*dx);
			}
			if((i/(Sx)%Sy)!=(Sy-1))
			{
				value[count++] = -sigma*dt/(dy*dy);
			}
			if(i/(Sx*Sy)!=(Sz-1))
			{
				value[count++] = -sigma*dt/(dz*dz);
			}
		}
	}

	int methodJacobi(double* x1, double* x2, double *b, double eps) // eps - точность вычислений, до которых выполн€ет метод
	{
		int iteration = 1;					// подсчет итераций нужных дл€ нахождени€ решени€
		double* diag = new double[N];       // diag - массив диагональных элементов матрицы ј
		double tmp;							// временна€ переменна€ 
		for (int i=0; i<N; i++)				// поиск диагональных элементов 
		{
			int j = this->rowIndex[i];
			while(this->col[j]<i)
			{
				j++;
			}
			if (this->col[j] == i)
				tmp = this->value[j];
			diag[i] = tmp;
		}									// диагоальные элементы найдены

		//double* Multip = new double[N];		// результат умножени€ матрицы на вектор
		double * Multip = this->Multiplicate(x1);	// умножаем матрицу ј на вектор х1
		for ( int i=0; i<N; i++)
		{
			x2[i] = x1[i] - (Multip[i] - b[i])/diag[i];
		}
		while (dev(x1,x2) > eps)
		{
			delete(Multip);
			for(unsigned int i=0; i<N; i++)
			{
				x1[i]=x2[i];
			}
			Multip = this->Multiplicate(x2);
			for ( int i=0; i<N; i++)	
			{
				x2[i] = x2[i] - (Multip[i] - b[i])/diag[i];
			}
			iteration++;
		}
		delete [] diag;
		return iteration;
	}
	protected:
		double dev(double* x1, double* x2)
		{
			double max = fabs(x1[0]-x2[0]);
			double tmp;
			for (int i=1; i<N; i++)
			{
				tmp = fabs(x1[i]-x2[i]);
				if (max > tmp) max = tmp;
			}
			return max;
		}
	public:
	~crsMatrix()
	{
		delete [] value;
		delete [] col;
		delete [] rowIndex;
	}

};


