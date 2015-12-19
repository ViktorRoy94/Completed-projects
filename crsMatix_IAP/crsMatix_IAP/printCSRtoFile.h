#include <fstream>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace std;

void printCSRtoFile(int n, int nnz, int* ia, int* ja, complex<double>* a, ofstream &output)
{
	cout.setf(ios::scientific, ios::floatfield);
	cout.precision(16);
	output << "rows: " << setw(10) << n << endl;
	output << "nnz : " << setw(10) << nnz << endl;

	if (nnz == n*n)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				output << setw(10) << a[i*n + j] << endl;
			}
			output << endl;
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			for (int index = ia[i]; index < ia[i + 1]; index++)
			{
				int j = ja[index];
				output << setw(10) << "(" << i << ", " << j << ") " << a[index] << endl;;
			}
			output << endl;
		}
	}
}