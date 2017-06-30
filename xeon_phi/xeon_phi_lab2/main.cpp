/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Виктор
 *
 * Created on April 16, 2016, 3:52 AM
 */

#include <cstdlib>
#include <time.h> 
#include <iostream> 
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int n = 40000;
    double *A = new double[n*n];
    double *b = new double[n];
    double *c = new double[n];

    srand(time(0));  
    for(int i = 0; i < n; i++) {
        b[i] = (double)rand() / RAND_MAX * 100;
        double sum = 0;
        for(int j = 0; j < n; j++) {
            A[i*n+j] = 1 + (double)rand() / RAND_MAX * 100;
            sum += A[i*n+j];
        }
        A[i*n+i] = sum * 3;
    }
    
    
    int start_time =  clock();
//    #pragma simd
    for (int i = 0; i < n; i++) {
        c[i] = 0;
        #pragma simd
        for (int j = 0; j < n; j++)
            c[i] += A[i*n+j]*b[i];
    }
    
    int time = clock() - start_time;
    cout << "time CPU = " << (float)time/CLOCKS_PER_SEC << endl;

    return 0;
}

