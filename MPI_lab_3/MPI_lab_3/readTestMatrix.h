
void readTestMatrix (int *rowIndex, int *col, double *value, double *x, int &n, int &nz)
{
	FILE *testMatrix;
	fopen_s(&testMatrix, "testMatrix.txt", "r");
	fscanf(testMatrix, "%d\n", &n);
	fscanf(testMatrix, "%d\n", &nz);
	
	for (int i = 0; i < n + 1; i++) {
		fscanf(testMatrix, "%d ", &rowIndex[i]);
	}
	fscanf(testMatrix, "\n");

	for (int i = 0; i < nz; i++) {
		fscanf(testMatrix, "%d ", &col[i]);
	}
	fscanf(testMatrix, "\n");
	
	for (int i = 0; i < nz; i++) {
		fscanf(testMatrix, "%.2lf ", &value[i]);
	}
	fscanf(testMatrix, "\n");

	for (int i = 0; i < n; i++) {
		fscanf(testMatrix, "%d ", &x[i]);
	}
	fscanf(testMatrix, "\n");
	fclose(testMatrix);
}