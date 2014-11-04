#include "CUDA.cuh"
 
 int flag = 0; // ���� � flag, ���� ���� �����, ������� 1 => �������� ����� x1 � �2 > eps
 int iteration = 1;						// ������� �������� ������ ��� ���������� �������     

 __global__ int methodJacobi(crsMatrix A, double* x1, double* x2, double *b, double eps,double diag) // eps - �������� ����������, �� ������� ��������� �����
	{
		
		double * Multip = A.Multiplicate(x1);	// �������� ������� � �� ������ �1
		for ( int i=0; i<N; i++)
		{
			x2[i] = x1[i] - (Multip[i] - b[i])/diag;
		}
		while (flag==1)
		{
			flag = 0;
			if (fabs(x1[i]-x2[i]) > eps) flag++;  // x1[i],x2[i] �������� �� x1[�����1] � �.�.
			
			delete(Multip);
			for(unsigned int i=0; i<N; i++)
			{
				x1[i]=x2[i];
			}
			Multip = A.Multiplicate(x2);
			for ( int i=0; i<N; i++)	
			{
				x2[i] = x2[i] - (Multip[i] - b[i])/diag;
			}
			iteration++;
		}
		return iteration;
	}


	/*double dev(double* x1, double* x2)
		{
			double max = fabs(x1[0]-x2[0]);
			double tmp;
			for (int i=1; i<N; i++)
			{
				tmp = fabs(x1[i]-x2[i]);
				if (max > tmp) max = tmp;
			}
			return max;
		}*/

void StartCuda(crsMatrix A, double* x1, double* x2, double *b, double eps, double diag)
{
	methodJacobi<<<Sy-2,Sz-2>>>(A, x1, x2, b, eps, diag);
	cudaDeviceSynchronize();
}