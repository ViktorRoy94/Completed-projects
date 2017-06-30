/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   methodJacobi_cilk.h
 * Author: Виктор
 *
 * Created on April 16, 2016, 2:46 AM
 */

#include <cilk/cilk.h>
#include <offload.h>
#include <cilk/reducer_opadd.h>


#ifndef METHODJACOBI_CILK_H
#define METHODJACOBI_CILK_H

double dev_cilk(double *A, double* x, double *b, int N);
void Ax_cilk(double *A, double *x, double *result, int n);
    
int methodJacobi_cilk(double* A, double* x, double *b, int N, double eps) {
    double* x1 = new double[N];
    double* x2 = new double[N];
    
    for (int i = 0; i < N; i++) {
        x1[i] = x[i];
    }
    do {
        double *multip = new double[N];
        Ax_cilk(A, x1, multip, N);
        cilk_for ( int i = 0; i < N; i++) {
            x2[i] = x1[i] - (multip[i] - b[i])/A[i*N+i];
        }
        for(unsigned int i = 0; i < N; i++) {
            x1[i] = x2[i];
        }

        delete(multip);
    }
    while (dev_cilk(A, x1, b, N) > eps);
    for (int i = 0; i < N; i++) {
        x[i] = x1[i];
    }
    return 0;
}

void Ax_cilk(double *A, double *x, double *result, int n) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++)
            result[i] += A[i*n+j]*x[j];
    }
}

double dev_cilk(double *A, double* x, double *b, int N) {
	double *multip = new double[N];
        Ax_cilk(A, x, multip, N);
        double max = fabs(multip[0]-b[0]);
        for (int i = 0; i < N; i++) {
            double tmp = fabs(multip[i]-b[i]);
            if (max > tmp) max = tmp;
        }
	return max;
}

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* METHODJACOBI_CILK_H */

