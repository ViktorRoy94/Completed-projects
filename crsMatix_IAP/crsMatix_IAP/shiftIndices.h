void shiftIndices(int n, int nonzeros, int* ia, int* ja, int value)
{
	int i;
	for (i = 0; i < n + 1; i++)
	{
		ia[i] += value;
	}
	for (i = 0; i < nonzeros; i++)
	{
		ja[i] += value;
	}
}