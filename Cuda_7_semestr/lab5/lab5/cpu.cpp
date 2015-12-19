#include "convolution.h"


void convolve(const cufftComplex * a, const cufftComplex * b, int n, cufftComplex * result)
{
    for (int i = 0; i < n; ++i)
    {
        result[i].x = 0;
        result[i].y = 0;
        for (int j = 0; j < n; ++j)
        {
			int index;
			if (i >= j) 
				index = i - j;
			else
				index = n + i - j;
            result[i].x += a[j].x * b[index].x - a[j].y * b[index].y;
            result[i].y += a[j].x * b[index].y + a[j].y * b[index].x;
        }
    }
}
