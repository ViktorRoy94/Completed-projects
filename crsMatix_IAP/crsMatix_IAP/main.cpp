#define _CRT_SECURE_NO_WARNINGS

// users header
#include "dumpCSR.h"
#include "crsMatrix.h"
#include "COOtoCRS.h"
#include "getSwap_table.h"
#include "transpositionIndicesCOO.h"
#include "printCSR.h"
#include "shiftIndices.h"
#include "getShurTest.h"
#include "printCSRtoFile.h"
#include "portable_time.h"

// standart header
#include <complex>
#include <cmath>
#include <stdlib.h>

using std::complex;
using std::vector;

/* PARDISO prototype. */
extern "C" void pardisoinit(void   *, int *, int *, int *, double *, int *);
extern "C" void pardiso(void   *, int *, int *, int *, int *, int *,
	complex<double> *, int *, int *, int *, int *, int *,
	int *, complex<double> *, complex<double> *, int *, double *);

extern "C" void pardiso_chkmatrix_z(int *, int *, complex<double> *, int *, int *, int *);
extern "C" void pardiso_chkvec_z(int *, int *, complex<double> *, int *);
extern "C" void pardiso_printstats_z(int *, int *, complex<double> *, int *, int *, int *,
	complex<double> *, int *);

extern "C" void pardiso_get_schur(void*, int*, int*, int*, complex<double>*, int*, int*);

int main()
{
	FILE *matrix;
	fopen_s(&matrix, "full.bin", "r+b");

	int Format = 0; 	// 0 - координатный формат
	int nRows = 0;		// Число строк
	int nCols = 0;		// Число столбцов

	int valuesSize = 0; 	//Кол-во ненулевых значений в матрице
	int rowIndexSize = 0;	//Кол-во элементов в массиве индексов строк
	int colIndexSize = 0;	//Кол-во элементов в массиве индексов столбцов

	vector <int> colIndexes;			//Массив индексов столбцов
	vector <int> rowIndexes;			//Массив индексов строк
	vector <complex<double>> values;	//Массив ненулевых значении

	fread(&Format, sizeof(int), 1, matrix);
	fread(&nRows, sizeof(int), 1, matrix);
	fread(&nCols, sizeof(int), 1, matrix);

	fread(&valuesSize, sizeof(int), 1, matrix);
	fread(&rowIndexSize, sizeof(int), 1, matrix);
	fread(&colIndexSize, sizeof(int), 1, matrix);
	
	colIndexes.resize(colIndexSize);
	rowIndexes.resize(rowIndexSize);
	values.resize(valuesSize);

	fread(colIndexes.data(), sizeof(int), colIndexSize, matrix);
	fread(rowIndexes.data(), sizeof(int), rowIndexSize, matrix);
	fread(values.data(), sizeof(complex<double>), valuesSize, matrix);

/*	rowIndexes = { 0, 0, 0, 1, 2, 2, 3, 4 };
	colIndexes = { 0, 2, 4, 1, 2, 4, 3, 4 };
	values =	 { 1, 3, 1, 2, 3, 4, 1, 3};

	nRows = 5; 
	nCols = 5;
	valuesSize = 8;
	colIndexSize = 8;
	rowIndexSize = 8;
*/
	int count = 0;
	vector <int> swap_table;
	getSwap_table(swap_table, nRows, count);


	FILE* output;
	FILE* output1;
	fopen_s(&output, "output.txt", "w");
	fopen_s(&output1, "output1.txt", "w");


	transpositoinIdiciesCOO(values, colIndexes, rowIndexes, swap_table, nRows, valuesSize);

	vector <int> rowIndex(nRows + 1);
	vector <int> colIndex(colIndexSize);

	COOtoCRS(rowIndexSize, colIndexSize, nRows, rowIndexes, colIndexes, values, rowIndex, colIndex);

	crsMatrix <complex<double>> A(nRows, valuesSize, values, colIndex, rowIndex);

	//A.printMatrix();
	//printCSR(nRows, rowIndex[nRows], rowIndex.data(), colIndex.data(), values.data());

	//printf("col      = ");
	//for (int i = 0; i < colIndexSize; i++){
	//	printf("%d ", colIndex[i]);
	//}
	//
	//printf("\nrowIndex = ");
	//for (int i = 0; i < nRows + 1; i++){
	//	printf("%d ", rowIndex[i]);
	//}

	//printf("\nvalue    = ");
	//for (int i = 0; i < valuesSize; i++){
	//	printf("%.2lf ", values[i]);
	//}


	//printf("\nindeces    = ");
	//for (int i = 0; i < nRows; i++){
	//	printf("%d ", swap_table[i]);
	//}
	//printf("\n");

	//A.ReorderQuadForm(swap_table);
	//A.printMatrix();


	/* Matrix data. */

	int    n = nRows;
	int    nnz = rowIndex[n];
	int    *ia = new int[n];
	int    *ja = new int[nnz];
	complex<double> *a = new complex<double>[nnz];

	for (int i = 0; i < n + 1; i++) {
		ia[i] = rowIndex[i];
	}

	for (int i = 0; i < nnz; i++) {
		a[i] = values[i];
		ja[i] = colIndex[i];
	}

	int      mtype = 6;        /* Real complex symmetric matrix */


	//		dumpCSR("matrix_sample_mtype_13.csr", n, ia, ja, a);
	//		printCSR(n, nnz, ia, ja, a);

	/* Internal solver memory pointer pt,                  */
	/* 32-bit: int pt[64]; 64-bit: long int pt[64]         */
	/* or void *pt[64] should be OK on both architectures  */
	void    *pt[64];

	/* Pardiso control parameters. */
	int      iparm[65];
	double   dparm[64];
	int      solver;
	int      maxfct, mnum, phase, error, msglvl;

	/* Number of processors. */
	int      num_procs;

	/* Auxiliary variables. */
	char    *var;
	int      i = 0;

	complex<double>   zdum;              /* Double dummy */
	int               idum;              /* Integer dummy. */

	/* -------------------------------------------------------------------- */
	/* ..  Setup Pardiso control parameters and initialize the solvers      */
	/*     internal adress pointers. This is only necessary for the FIRST   */
	/*     call of the PARDISO solver.                                      */
	/* ---------------------------------------------------------------------*/

	error = 0;
	solver = 0; /* use sparse direct solver */
	pardisoinit(pt, &mtype, &solver, &iparm[1], dparm, &error);

	if (error != 0)
	{
		if (error == -10)
			printf("No license file found \n");
		if (error == -11)
			printf("License is expired \n");
		if (error == -12)
			printf("Wrong username or hostname \n");
		return 1;
	}
	else
		printf("[PARDISO]: License check was successful ... \n");


	/* Numbers of processors, value of OMP_NUM_THREADS */
	var = getenv("OMP_NUM_THREADS");
	if (var != NULL)
		sscanf(var, "%d", &num_procs);
	else
	{
		printf("Set environment OMP_NUM_THREADS to 1");
		exit(1);
	}
	iparm[3] = num_procs;
	iparm[11] = 1;		//Use (non-) symmetric scaling vectors.	iparm[13] = 0;


	maxfct = 1;         /* Maximum number of numerical factorizations.  */
	mnum = 1;         /* Which factorization to use. */

	msglvl = 1;         /* Print statistical information  */
	error = 0;         /* Initialize error flag */


	/* -------------------------------------------------------------------- */
	/* ..  Convert matrix from 0-based C-notation to Fortran 1-based        */
	/*     notation.                                                        */
	/* -------------------------------------------------------------------- */
	shiftIndices(n, nnz, ia, ja, 1);


	/* -------------------------------------------------------------------- */
	/*  .. pardiso_chk_matrix(...)                                          */
	/*     Checks the consistency of the given matrix.                      */
	/*     Use this functionality only for debugging purposes               */
	/* -------------------------------------------------------------------- */
	pardiso_chkmatrix_z(&mtype, &n, a, ia, ja, &error);
	if (error != 0)
	{
		printf("\nERROR in consistency of matrix: %d", error);
		exit(1);
	}


	/* -------------------------------------------------------------------- */
	/* ..  Reordering and Symbolic Factorization.  This step also allocates */
	/*     all memory that is necessary for the factorization.              */
	/* -------------------------------------------------------------------- */

	int nrows_S =  count;
	phase = 12;
	iparm[38] = nrows_S;

	int nb = 0;

	double time = PortableGetTime();

	pardiso(pt, &maxfct, &mnum, &mtype, &phase,
		&n, a, ia, ja, &idum, &nb,
		&iparm[1], &msglvl, &zdum, &zdum, &error, dparm);

	if (error != 0)
	{
		printf("\nERROR during symbolic factorization: %d", error);
		exit(1);
	}
	printf("\nReordering completed ...\n");
	printf("Number of nonzeros in factors  = %d\n", iparm[18]);
	printf("Number of factorization MFLOPS = %d\n", iparm[19]);
	printf("Number of nonzeros is   S      = %d\n", iparm[39]);

	/* -------------------------------------------------------------------- */
	/* ..  allocate memory for the Schur-complement and copy it there.      */
	/* -------------------------------------------------------------------- */
	int nonzeros_S = iparm[39];

	int* iS = new int[nrows_S + 1];
	int* jS = new int[nonzeros_S];
	complex<double>* S = new complex<double>[nonzeros_S];

	pardiso_get_schur(pt, &maxfct, &mnum, &mtype, S, iS, jS);

	time = PortableGetTime() - time;
	/* -------------------------------------------------------------------- */
	/* ..  Convert matrix from 1-based Fortan notation to 0-based C         */
	/*     notation.                                                        */
	/* -------------------------------------------------------------------- */
	shiftIndices(n, nnz, ia, ja, -1);

	/* -------------------------------------------------------------------- */
	/* ..  Convert Schur complement from Fortran notation held internally   */
	/*     to 0-based C notation                                            */
	/* -------------------------------------------------------------------- */
	shiftIndices(nrows_S, nonzeros_S, iS, jS, -1);
	
	//printCSR(nrows_S, nonzeros_S, iS, jS, S);

	/* -------------------------------------------------------------------- */
	/* ..  Termination and release of memory.                               */
	/* -------------------------------------------------------------------- */
	phase = -1;                 /* Release internal memory. */

	pardiso(pt, &maxfct, &mnum, &mtype, &phase,
		&n, &zdum, ia, ja, &idum, &idum,
		&iparm[1], &msglvl, &zdum, &zdum, &error, dparm);

	vector <complex<double>> shurTest (count * count);
	getShurTest(shurTest, count);

	ofstream shur1;
	ofstream shur2;
	shur1.open("shurTest.txt");
	shur2.open("shurPardiso.txt");

	cout.setf(ios::scientific, ios::floatfield);
	cout.precision(16);
	//shur1 << "rows: " << setw(10) << count << endl;
	//shur1 << "nnz : " << setw(10) << nonzeros_S << endl;

	vector <complex<double>> shurPardiso(count * count);

	for (int i = 0; i < count; i++)
	{
		for (int index = iS[i]; index < iS[i + 1]; index++)
		{
			int j = jS[index];
			shurPardiso[i*count + j] = S[index];
			if (j > 0 && i <= j) {
				shurPardiso[j*count + i] = S[index];
			}
		}
	}
	complex <double> sum;
	complex <double> sum1;

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (i > 0 && j <= i) {
				shurTest[j*count + i] = shurTest[i*count + j];
			}
		}
	}

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			sum += (shurPardiso[i*count + j] - shurTest[i*count + j]) * (shurPardiso[i*count + j] - shurTest[i*count + j]);
			sum1 += (shurTest[i*count + j]) * (shurTest[i*count + j]);
		}
	}

	cout << sum / sum1 << endl;
	/*for (int i = 0; i < nonzeros_S; i++)
	{
		shur1 << setw(10) << iS[i];
	}
	shur1 << endl;

	for (int i = 0; i < count; i++)
	{
		shur1 << setw(10) << jS[i];
	}
	shur1 << endl;

	for (int i = 0; i < count; i++)
	{
		shur1 << setw(10) << S[i];
	}*/


	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++){
			shur1 << setw(20) << shurTest[j*count + i].real() << ' ';
		}
		shur1 << endl;
	}
	shur1 << endl;
	
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++){
			shur2 << setw(20) << shurPardiso[i*count + j].real() << ' ';
		}
		shur2 << endl;
	}
	shur2 << endl;

	cout << time << endl;
	shur1.close();
	shur2.close();

	delete[] iS;
	delete[] jS;
	delete[] S;


	char c = 0;
	scanf_s("%d", c);
	
	fclose(output);
}