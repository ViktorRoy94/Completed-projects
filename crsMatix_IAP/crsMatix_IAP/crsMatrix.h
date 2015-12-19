#include <stdio.h>
#include <string>
#include <vector>
#include <complex>
#include "compareItems.h"

using std::vector;
using std::complex;

template< class T>
class crsMatrix
{
private:
	int N;					// размер матрицы
	int NZ;					// кол-во ненулевых элементов
	vector<T> value;		// массив значений (размер NZ)
	vector<int> col;		// массив номеров столбцов (размер NZ)
	vector<int> rowIndex;	// массив индексов строк (размер N+1)

public:
	crsMatrix(int n, int nz, const vector<complex<double>> _value, const vector<int> _col,
					         const vector<int> _rowIndex) 
	{
		N = n;
		NZ = nz;
		
		for (int i = 0; i < NZ; i++) {
			value.push_back(_value[i]);
		}
		
		for (int i = 0; i < NZ; i++) {
			col.push_back(_col[i]);
		}

		for (int i = 0; i < N + 1; i++) {
			rowIndex.push_back(_rowIndex[i]);
		}
	}

public:
	void ReorderQuadForm(const vector<int> swap_table) 
	{
		auto &qf_val_c = value;
		auto &qf_col   = col;
		auto &qf_rowi  = rowIndex;
		int n_tot = qf_rowi.size() - 1;
		int j;

		vector<int> pnSortArrC(2 * n_tot); 
		vector<int> pnSortArrR(2 * n_tot);
		// - массивы дл€ сортировки (новый номер, стара€ позици€)
		int n_old_row;

		//сортируем строки
		for (j = 0; j < n_tot; j++) {
			pnSortArrR[2 * j] = -1;
		}

		for (n_old_row = 0; n_old_row<n_tot; n_old_row++) {
			int n_new_row = swap_table[n_old_row];
			pnSortArrR[2 * n_new_row] = n_new_row;
			pnSortArrR[2 * n_new_row + 1] = n_old_row;
		}
		
		qsort(pnSortArrR.data(), n_tot, 2 * sizeof(int), comp_int);
		
		//проверка
		for (j = 0; j < n_tot; j++) {
			if (pnSortArrR[2 * j] < 0) break;
		}
		if (j < n_tot) {
			printf("ReorderQuadForm: caught a silly error (4)");
			printf("¬нутренн€€ ошибка.");
		}
		
		//pxQF_val, pnQF_col, pnQF_rowi - данные в старом пор€дке
		vector<T>	   pxQF_val  = qf_val_c;  //qf_val_c,... - результат перестановки
		vector<int>    pnQF_col  = qf_col;
		vector<int>    pnQF_rowi = qf_rowi;
		
		qf_rowi[0] = 0;
		int n_new_pos = 0;

		//копируем строки в новое место с сортировкой внутри строки
		for (int n_new_row = 0; n_new_row < n_tot; n_new_row++)
		{
			n_old_row = pnSortArrR[2 * n_new_row + 1];				  // номер столбца матрицы результата
			
			//сортировка
			int n_old_start = pnQF_rowi[n_old_row];					  // начало n_old_row строки
			int N_row_len = pnQF_rowi[n_old_row + 1] - n_old_start;   // длина n_old_row строки
			for (j = 0; j < N_row_len; j++) {
				int pos = j + n_old_start;							  // 
				pnSortArrC[2 * j] = swap_table[pnQF_col[pos]];			
				pnSortArrC[2 * j + 1] = pos;
			}
			
			qsort(pnSortArrC.data(), N_row_len, 2 * sizeof(int), comp_int);
			
			//проверка
			if ((pnSortArrC[0]<0) && (N_row_len>0)) {
				printf("ReorderQuadForm: caught a silly error (5)");
				printf("¬нутренн€€ ошибка.");
			}

			//запись
			for (j = 0; j<N_row_len; j++)
			{
				qf_col[n_new_pos] = pnSortArrC[2 * j];
				qf_val_c[n_new_pos] = pxQF_val[pnSortArrC[2 * j + 1]];
				n_new_pos++;
			}
			qf_rowi[n_new_row + 1] = n_new_pos;
		}//n_new_row
		//проверка
		int qf_nnz = pxQF_val.size();
		if (n_new_pos != qf_nnz) {
			printf("ReorderQuadForm: caught a silly error (6)");
			printf("¬нутренн€€ ошибка.");
		}
	}

	void printMatrix ()
	{
		for (int i = 0; i < N; i++) {
			for (int k = rowIndex[i]; k < rowIndex[i + 1]; k++) 
			{
			
				if (k == rowIndex[i]) {
					for (int j = 0; j < col[k]; j++){
						printf("0.00  ");
					}
				}
				
				//printf("%.2lf ", value[k]);
				
				cout <<  value[k] << ' ';

				if (k == rowIndex[i + 1] - 1) {
					for (int j = col[k]; j < N - 1; j++){
						printf("0.00  ");
					}
					continue;
				}
				
				for (int j = col[k]+1; j < col[k+1]; j++) {
					printf("0.00  ");
				}

			}
			printf("\n");
		}
	}

	void printMatrixToFile(FILE* output)
	{
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
	}

};


