/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Виктор
 *
 * Created on April 16, 2016, 1:57 AM
 */

#include <cstdlib>
#include <time.h> 
#include <iostream>
#include "methodJacobi_CPU.h"
#include "methodJacobi_OMP.h"
#include "methodJacobi_cilk.h"

using namespace std;

/*
 * 
 */
int main() {
    int n = 25000;
    double eps = 0.001;
    double *b = new double [n];
    double *x = new double [n];
    double *A = new double [n*n];	
    srand(time(0));  
    for(int i = 0; i < n; i++) {
        b[i] = (double)rand() / RAND_MAX * 100;
        x[i] = b[i];
        double sum = 0;
        for(int j = 0; j < n; j++) {
            A[i*n+j] = 1 + (double)rand() / RAND_MAX * 100;
            sum += A[i*n+j];
        }
        A[i*n+i] = sum * 3;
    }
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++)
//            cout << A[i*n+j] << " ";
//        cout << endl;
//    }
    //CPU
    int start_time =  clock();
    methodJacobi_cpu(A, x, b, n, eps);
    int time = clock() - start_time;
    cout << "time CPU = " << (float)time/CLOCKS_PER_SEC << endl;
    cout << "|Ax-b| = " << dev_cpu(A, x, b, n) << endl;
    
    start_time =  clock();
    #pragma offload target(mic) in(A:length(n*n)) in(b:length(n)) inout(x:length(n))
    methodJacobi_omp(A, x, b, n, eps);
    time = clock() - start_time;
    cout << "time OMP = " << (float)time/CLOCKS_PER_SEC << endl;
    cout << "|Ax-b| = " << dev_cpu(A, x, b, n) << endl;
    
    start_time =  clock();
    methodJacobi_cilk(A, x, b, n, eps);
    time = clock() - start_time;
    cout << "time cilk = " << (float)time/CLOCKS_PER_SEC << endl;
    cout << "|Ax-b| = " << dev_cpu(A, x, b, n) << endl;
    
    return 0;
}



