#ifndef CYCLE_REDUCTION
#define CYCLE_REDUCTION
#include <QDebug>
int cycle_reduction(double *x, double a0, double a1, double a2,
                    double *f, int n, int q);

int cycle_reduction_parallel(double *x, double a0, double a1, double a2,
                    double *f, int n, int q, int threads);
#endif // CYCLE_REDUCTION

