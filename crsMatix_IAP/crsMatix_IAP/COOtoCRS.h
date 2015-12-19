#include <armadillo>
#include <vector>
using std::vector;
using std::complex;

void COOtoCRS(const int rowIndexSize,
			  const int colIndexSize,
			  const int nRows,
			  const vector <int> &rowIndexes,
			  const vector <int> &colIndexes,
			  vector <complex<double>> &values,
			  vector <int> &rowIndex,
			  vector <int> &colIndex
			  )
{

	arma::umat locations(2, rowIndexSize);

	for (int i = 0; i < rowIndexSize; i++)
	{
		locations(1, i) = rowIndexes[i];
		locations(0, i) = colIndexes[i];
	}

	arma::cx_vec valuesArma(values);
	arma::sp_cx_mat sparseMatrix(locations, valuesArma, true);
	//sparseMatrix.print();

	for (int i = 0; i < nRows + 1; i++) {
		rowIndex[i] = sparseMatrix.col_ptrs[i];
	}

	for (int i = 0; i < colIndexSize; i++) {
		colIndex[i] = sparseMatrix.row_indices[i];
	}
	for (int i = 0; i < colIndexSize; i++) {
		values[i] = sparseMatrix.values[i];
	}

}