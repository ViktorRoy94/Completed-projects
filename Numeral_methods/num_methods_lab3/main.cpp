#include <cmath>
#include <iostream>
#include <armadillo>
#include <omp.h>
#include "cycle_reduction.h"

using namespace std;
using namespace arma;

double u_(double x, double t) {
    return sin(x*t);
}

double dudt(double x, double t) {
    return x*cos(x*t);
}

double ddu(double x, double t) {
    return -t*t*sin(x*t);
}

double f(double x, double t) {
    return (dudt(x,t) - ddu(x,t));
}

int main()
{
    int nx = 1024*16*4;
    int nt = 1000*4;
    double tau = (double)1/nt;
    double h = (double)1/nx;
    double sigma = 0.5;
    double k = tau/(h*h);

//    cout << tau << " " << h << " " << k << endl;

    double a0 = -sigma*k;
    double a1 = 1+2*sigma*k;
    double a2 = -sigma*k;

    double b0 = (1-sigma)*k;
    double b1 = 1-2*sigma*k;
    double b2 = (1-sigma)*k;

//    mat A(nx-1,nx-1);
//    for(int i = 0; i < nx-1; i++){
//            A(i,i) = a1;
//            if(i>0)
//                A(i,i-1) = a0;
//            if(i<nx-2)
//                A(i,i+1) = a2;
//        }
//    matrix.print();
    double *x_prev = new double[nx+1];
    double *x_next = new double[nx+1];
    for (int threads = 0 ; threads < 5; threads += 2) {

        double time1 = omp_get_wtime();

        for(int i = 0; i < nx+1; i++) {
            x_prev[i] = u_(i*h,0);
        }

        for(int t = 1; t < nt; t++) {
            x_next[0] = u_(0, t*tau);
            x_next[nx] = u_(h*nx, t*tau);

            // правая часть
            double* b = new double[nx+1];
            for(int i = 1; i < nx; i++) {
                b[i] = 0;
                b[i] += x_prev[i-1]*b0;
                b[i] += x_prev[i  ]*b1;
                b[i] += x_prev[i+1]*b2;
                b[i] += tau*f((i-1)*h,(t+0.5)*tau);
            }
            b[1] -= a0*x_next[0];
            b[nx-1] -= a2*x_next[nx];

//            vec b_ (nx-1);
//            for(int i = 0; i < nx-1; i++) {
//                b_[i] = b[i+1];
//            }
//            vec x_ = solve(A,b_);
            int q = (int)(log((double)nx) / log(2.0));
            cycle_reduction_parallel(x_next,a0,a1,a2,b,nx,q,threads);

//            for(int i = 0; i < nx-1; i++) {
////                cout << abs(x_next[i+1]-x_[i]) << endl;
//                x_next[i+1] = x_[i];
//            }

            double* temp = x_next;
            x_next = x_prev;
            x_prev = temp;
            delete [] b;
        }
        double time2 = omp_get_wtime();

        double diff = .0;
        for(int i = 0; i < nx+1; i++) {
            double temp = abs(x_prev[i] - u_(i*h,(nt-1)*tau));
//            cout << x_prev[i] << " " << u_(i*h,(nt-1)*tau) << endl;
            if (temp > diff)
                diff = temp;
        }
        cout << "threads = " << threads << " time = " << time2-time1 << " diff = " << diff << endl;
    }
    delete [] x_next;
    delete [] x_prev;
}
