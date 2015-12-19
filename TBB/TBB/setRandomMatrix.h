#include <stdio.h>
#include <string>
#include <omp.h>

#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"


class CCSmatrix
{
	double *value_;
	int *colIndex_;
	int *row_;
	double *b_;
	double *x_;
	int n_; 
	int nz_;
	int nzInString_;

public:	
	CCSmatrix(int n, int nzInString)
	{
		nz_ = nzInString * n;
		n_ = n;
		nzInString_ = nzInString;
		value_ = new double[nz_];
		row_ = new int[nz_];
		colIndex_ = new int[n_ + 1];
		b_ = new double[n];
		x_ = new double[n];
	}
	
	~CCSmatrix()
	{
		delete [] value_;
		delete [] row_;
		delete [] colIndex_;
	}

	void parallelSection(int i)
	{
		for (int j = colIndex_[i]; j < colIndex_[i + 1]; j++) {
			b_[row_[j]] += value_[j] * x_[i];
		}
	}

	void operator () (tbb::blocked_range<int>& r)
	{
	/*	tbb::parallel_reduce(tbb::blocked_range<int>(0, n_),
			[=](const tbb::blocked_range<int>& r) {
			for (int i = r.begin(); i != r.end(); ++i)
			{
				parallelSection(i);
			}
		}
		);*/
		for (int i = r.begin(); i != r.end(); ++i)
		{
			for (int j = colIndex_[i]; j < colIndex_[i + 1]; j++) {
				b_[row_[j]] += value_[j] * x_[i]; 
			}
		}

	}

	void join(const CCSmatrix& obj)
	{
		for (int i = 0; i < n_; i++){
			b_[i] += obj.b_[i];
		}
	}

	CCSmatrix(double tmpb[]) : b_(tmpb) {}


	//void multiplicate(double* b, double* x)
	//{
	//	for (int i = 0; i < n_; i++) {
	//		for (int j = colIndex_[i]; j < colIndex_[i + 1]; j++) {
	//			b[row_[j]] += value_[j] * x[i];
	//		}
	//	}
	//}

	void setRandomMatrix()
	{
		for (int i = 0; i < n_; i++)
		{
			x_[i] = rand() % 10 + 1;
			b_[i] = 0;
		}
		
		// заполняем случайно вектор значений
		#pragma parallel for
		for (int i = 0; i < nz_; i++) {
			value_[i] = rand() % 10 + 1;
		}

		// заполняем вектор индексов строк
		#pragma omp parallel for
		for (int i = 0; i < n_ + 1; i++)
		{
			colIndex_[i] = i * nzInString_;
		}

		// св-ва массива rowIndex
		colIndex_[0] = 0;
		colIndex_[n_] = nz_;

		// заполняем col неповторяющимися значениями в j строке 
		memset(row_, 0, nz_*sizeof(int));
		#pragma omp parallel for
		for (int j = 0; j < n_; j++)
		{
			for (int k = colIndex_[j]; k < colIndex_[j + 1]; k++)
			{
				do
				{
					int R1 = rand() % n_;
					int R2 = rand() % n_;
					double temp = (double)(R1 * R2) / (RAND_MAX * RAND_MAX);
					row_[k] = (int)(temp * n_);
				} while (prov(colIndex_[j], k, row_, row_[k]));
			}
		}

		// выстраиваем значения col по возрастанию в каждой j строке
		for (int i = 0; i < n_; i++)
		{
			for (int k = colIndex_[i]; k < colIndex_[i + 1]; k++)
			{
				for (int j = colIndex_[i + 1] - 1; j > k; j--)
				{
					if (row_[j - 1] > row_[j])
					{
						int tmp = row_[j];
						row_[j] = row_[j - 1];
						row_[j - 1] = tmp;
					}
				}
			}
		}
	}

	void printVectorToFile(const double *b, int N, FILE* output)
	{
		for (int i = 0; i < N; i++)
		{
			fprintf(output, "%lf\n", b[i]);
		}
	}

	void printVectorToFile(FILE* output)
	{
		for (int i = 0; i < n_; i++)
		{
			fprintf(output, "%d\n", b_[i]);
		}
	}




protected:	
	int prov(int start, int end, int *mas, int digit)
	{
		for (int i = start; i < end; i++)
		if (mas[i] == digit) return 1;
		return 0;
	}
};