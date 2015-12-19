#include <vector>
#include <complex>
using std::vector;
using std::complex;

void transpositoinIdiciesCOO(vector <complex<double>> &values,
							 vector <int>  &col,
							 vector <int>  &row, 
							 const vector <int> &swap_table, 
							 const int n, 
							 const int nz)
{
	for (int i = 0; i < nz; i++) {
		col[i] = swap_table[col[i]];
		row[i] = swap_table[row[i]];
		if (row[i] > 1 && col[i] <= row[i]) {
			swap(col[i], row[i]);
		}
	}
}