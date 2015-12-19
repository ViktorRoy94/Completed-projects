#include <fstream>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;

void printCSR(int n, int nnz, int* ia, int* ja, complex<double>* a)
{
	cout.setf(ios::scientific, ios::floatfield);
	cout.precision(16);
	cout << "rows: " << setw(10) << n << endl;
	cout << "nnz : " << setw(10) << nnz << endl;

	if (nnz == n*n)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << setw(10) << a[i*n + j] << endl;
			}
			cout << endl;
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			for (int index = ia[i]; index < ia[i + 1]; index++)
			{
				int j = ja[index];
				cout << setw(10) << "(" << i << ", " << j << ") " << a[index];
			}
			cout << endl;
		}
	}
}