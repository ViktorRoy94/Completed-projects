/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   methodJacobi_OMP.h
 * Author: Виктор
 *
 * Created on April 16, 2016, 2:46 AM
 */

#ifndef METHODJACOBI_OMP_H
#define METHODJACOBI_OMP_H

__attribute__((target(mic))) double dev_omp(double *A, double* x, double *b, int N);
__attribute__((target(mic))) void Ax_omp(double *A, double *x, double *result, int n);
    
__attribute__((target(mic))) int methodJacobi_omp(double* A, double* x, double *b, int N, double eps) {
    double* x1 = new double[N];
    double* x2 = new double[N];
    
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        x1[i] = x[i];
    }
    do {
        double *multip = new double[N];
        Ax_omp(A, x1, multip, N);
        #pragma omp parallel for
        for ( int i = 0; i < N; i++) {
            x2[i] = x1[i] - (multip[i] - b[i])/A[i*N+i];
        }
        #pragma omp parallel for
        for(unsigned int i = 0; i < N; i++) {
            x1[i] = x2[i];
        }

        delete(multip);
    }
    while (dev_omp(A, x1, b, N) > eps);
    
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        x[i] = x1[i];
    }
    return 0;
}

__attribute__((target(mic))) void Ax_omp(double *A, double *x, double *result, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++)
            result[i] += A[i*n+j]*x[j];
    }
}

__attribute__((target(mic))) double dev_omp(double *A, double* x, double *b, int N) {
	double *multip = new double[N];
        Ax_omp(A, x, multip, N);
        double max = fabs(multip[0]-b[0]);
        for (int i = 0; i < N; i++) {
            double tmp = fabs(multip[i]-b[i]);
            if (max > tmp) max = tmp;
        }
	return max;
}

#endif /* METHODJACOBI_OMP_H */

