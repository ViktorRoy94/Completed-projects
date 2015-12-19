
void printTestMatrix (const int *rowIndex, const int *col, const double *value, const double *x, const int n, const int nz)
{
	FILE *testMatrix;
	fopen_s(&testMatrix, "testMatrix.txt", "w");

	fprintf(testMatrix,"%d\n", n);
	fprintf(testMatrix,"%d\n", nz);

	for (int i = 0; i < n + 1; i++) {
		fprintf(testMatrix, "%d ", rowIndex[i]);
	}
	fprintf(testMatrix, "\n");

	for (int i = 0; i < nz; i++) {
		fprintf(testMatrix, "%d ", col[i]);
	}
	fprintf(testMatrix, "\n");
	
	for (int i = 0; i < nz; i++) {
		fprintf(testMatrix, "%.2lf ", value[i]);
	}
	fprintf(testMatrix, "\n");

	for (int i = 0; i < n; i++) {
		fprintf(testMatrix, "%d ", x[i]);
	}
	fprintf(testMatrix, "\n");
	fclose(testMatrix);
}