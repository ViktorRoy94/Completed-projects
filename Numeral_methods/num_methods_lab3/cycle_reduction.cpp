#include "cycle_reduction.h"

int cycle_reduction(double *x, double a0, double a1, double a2,
                    double *f, int n, int q)
{

    double *a = new double[q];
    double *b = new double[q];
    double *c = new double[q];
    a[0] = a0;
    b[0] = a1;
    c[0] = a2;
    f[0] = 0;
    f[n] = 0;
    double x0 = x[0];
    double xn = x[n];
    x[0] = 0;
    x[n] = 0;
    int start = 2;
    int elementsNum = n;
    int step = 1;
    for (int j = 0; j < q - 1; j++)
    {
        double alpha = -a[j] / b[j];
        double beta  = -c[j] / b[j];
        a[j+1] = alpha * a[j];
        b[j+1] = b[j] + 2 * alpha * c[j];
        c[j+1] = beta * c[j];
        elementsNum = (elementsNum - 1) / 2;
        for (int i = 0; i < elementsNum; i++)
        {
            int k = start * (i + 1);
            f[k] = alpha * f[k - step] + f[k] + beta * f[k + step];
        }
        start *= 2;
        step  *= 2;
    }
    start = n / 2;
    step  = start;
    elementsNum = 1;
    for (int j = q - 1; j >= 0; j--)
    {
        double alpha = -a[j] / b[j];
        double beta  = -c[j] / b[j];
        for (int i = 0; i < elementsNum; i++)
        {
            int k = start * (2 * i + 1);
            x[k] = f[k] / b[j] + alpha * x[k - step] + beta * x[k + step];
        }
        start /= 2;
        step = start;
        elementsNum *= 2;
    }
    x[0] = x0;
    x[n] = xn;
    delete [] c;
    delete [] b;
    delete [] a;
    return 0;
}

int cycle_reduction_parallel(double *x, double a_0, double a_1, double a_2,
                             double *f, int n, int q, int threads)
{
    double *a = new double[q];
    double *b = new double[q];
    double *c = new double[q];
    a[0] = a_0;
    b[0] = a_1;
    c[0] = a_2;
    f[0] = 0;
    f[n] = 0;
    double x0 = x[0];
    double xn = x[n];
    x[0] = 0;
    x[n] = 0;
    int start = 2;
    int elementsNum = n;
    int step = 1;
    for (int j = 0; j < q - 1; j++)
    {
        double alpha = -a[j] / b[j];
        double beta  = -c[j] / b[j];
        a[j + 1] = alpha * a[j];
        b[j + 1] = b[j] + 2 * alpha * c[j];
        c[j + 1] = beta * c[j];
        elementsNum = (elementsNum - 1) / 2;
        #pragma omp parallel for num_threads(threads)
        for (int i = 0; i < elementsNum; i++)
        {

            int k = start * (i + 1);
            f[k] = alpha * f[k - step] + f[k] + beta * f[k + step];
        }
        start *= 2;
        step  *= 2;
    }
    start = n / 2;
    step  = start;
    elementsNum = 1;
    for (int j = q - 1; j >= 0; j--)
    {
        double alpha = -a[j] / b[j];
        double beta  = -c[j] / b[j];
        #pragma omp parallel for num_threads(threads)
        for (int i = 0; i < elementsNum; i++)
        {
            int k = start * (2 * i + 1);
            x[k] = f[k] / b[j] + alpha * x[k - step] + beta * x[k + step];
        }
        start /= 2;
        step = start;
        elementsNum *= 2;
    }
    x[0] = x0;
    x[n] = xn;
    delete [] c;
    delete [] b;
    delete [] a;
    return 0;
}
