#include "stdio.h"
#include "string.h"
#include "portable_time.h"
#include <time.h>
#include <stdlib.h>

const int n = 100;

int main()
{
	int *a;			// matrix A
	int *b;			// matrix B
	int *c;			// matrix C
	FILE* output = fopen("output.txt","w");
	
	a = new int[n*n];
	b = new int[n*n];
	c = new int[n*n];

	/*for (int i = 0; i < n*n; i++)
	{
		a[i] = i + 1;
		b[i] = n*n - i;
	}
*/
	for (int i = 0; i < n*n; i++)
	{
		a[i] = rand() % 50;
		b[i] = rand() % 50;
	}

	memset(c, 0, n*n*sizeof(int));
	
	double time = PortableGetTime();
	for (int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
			{
				c[i*n+j] += a[i*n+k] * b[k*n + j];
			}
	
	time = PortableGetTime() - time; 
	fprintf(output,"\n");
	for (int i = 0; i < n; i++)
	{
		
		for (int j = 0; j < n; j++)
			fprintf(output,"%d ", c[i*n+j]);
		fprintf(output,"\n");
	}
	fprintf(output,"\ntime = %lf", time);
}