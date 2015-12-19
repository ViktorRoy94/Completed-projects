class crsMatrix
{
private:
	int N; // размер матрицы
	int NZ;  // кол-во ненулевых элементов
	double *value; // массив значений (размер NZ)
	int *col; // массив номеров столбцов (размер NZ)
	int *rowIndex; // массив индексов строк (размер N+1)

public:
	crsMatrix(int n, int nz, double *_value, int *_col, int *_rowIndex)
	{
		N = n;
		NZ = nz;
		
		value = new double[NZ];
		for (int i = 0; i<NZ; i++)
			value[i] = _value[i];
		
		col = new int[NZ];
		for (int i = 0; i<NZ; i++)
			col[i] = _col[i];
		
		rowIndex = new int[N+1];
		for (int i = 0; i<N+1; i++)
			rowIndex[i] = _rowIndex[i];
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
	

	void printMatrix ()
	{
		for (int i = 0; i < N; i++) {
			for (int k = rowIndex[i]; k < rowIndex[i + 1]; k++) {

				if (k == rowIndex[i]) {
					for (int j = 0; j < col[k]; j++){
						printf("0.00 ");
					}
				}

				printf("%.2lf ", value[k]);

				if (k == rowIndex[i+1]-1) {
					for (int j = col[k]; j < N-1; j++){
						printf("0.00 ");
					}
                                        continue;
				}

				for (int j = col[k]+1; j < col[k+1]; j++) {
					printf("0.00 ");
				}


			}
			printf("\n");
		}
	}

	void printMatrixToFile()
	{
		FILE* output;
		fopen_s(&output, "output.txt", "w");
		for (int i = 0; i < N; i++)
		{
			for (int k = rowIndex[i]; k < rowIndex[i + 1]; k++)
			{

				if (k == rowIndex[i]) {
					for (int j = 0; j < col[k]; j++){
						fprintf(output, "0.00 ");
					}
				}

				fprintf(output, "%.2lf ", value[k]);

       				if (k == rowIndex[i + 1] - 1) {
                                	for (int j = col[k]; j < N - 1; j++){
						fprintf(output, "0.00 ");
					}
                                        continue;
				}

                                for (int j = col[k] + 1; j < col[k + 1]; j++) {
					fprintf(output, "0.00 ");
				}


			}
			fprintf(output, "\n");
		}

	}

	~crsMatrix()
	{
		delete [] value;
		delete [] col;
		delete [] rowIndex;
	}

};


