class crsMatrix
{
private:
	int N; // ������ �������
	int NZ;  // ���-�� ��������� ���������
	double *value; // ������ �������� (������ NZ)
	int *col; // ������ ������� �������� (������ NZ)
	int *rowIndex; // ������ �������� ����� (������ N+1)

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
		double *b = new double[N];      // ��������� ��������� ������� �� ������
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


	~crsMatrix()
	{
		delete [] value;
		delete [] col;
		delete [] rowIndex;
	}

};


