#include "multiplication.h"

void multiplication(const int * a, const int * b, int * c, int m, int n, int k)
{
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < k; ++j)
        {
            c[i*k + j] = 0;
            for (int l = 0; l < n; ++l)
                c[i*k + j] += a[i*n + l] * b[l*k + j]; 
        }
}
