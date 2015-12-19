#include <complex>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std; 


void dumpCSR(const char* filename, int n, int* ia, int* ja, complex<double>* a)
{
	fstream fout(filename, ios::out);
	fout << n << endl;
	fout << n << endl;
	fout << ia[n] << endl;

	for (int i = 0; i <= n; i++)
	{
		fout << ia[i] << endl;
	}

	for (int i = 0; i < ia[n]; i++)
	{
		fout << ja[i] << endl;
	}

	for (int i = 0; i < ia[n]; i++)
	{
		fout << a[i] << endl;
	}

	fout.close();
}