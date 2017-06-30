#include <QDebug>
#include <ctime>

#include "func.h"

void monteCarloMethod(const QVector<double>& A, const QVector<double>& H,
                      const QVector<double>& b, const QVector<double>& c,
                      const int rectSize, QVector<double> &result, const int iterCount) {
    int n = c.size();
    double minFunc = 10000000.0;
    QVector <double> minX(n);
    //srand(time(NULL));
    for (int iteration = 0; iteration < iterCount; iteration++) {
        QVector <double> X(n);
        for (int i = 0; i < n; i++) {
            X[i] = ((double) rand() / (RAND_MAX))*rectSize;
        }
//        qDebug() << "X = " << X;
        // Удовлетворяет ли условиям Ax<=b
        int flag = 0;
        for (int i = 0; i < n; i++) {
            double temp = 0.0;
            for (int j = 0; j < n; j++) {
                temp += A[i*n+j]*X[j];
            }
            if (temp > b[i]) {
                flag = 1;
                break;
            }
        }

        if (flag)
            continue;

        double func_= func(X, H, c);
        if (func_ < minFunc) {
            minFunc = func_;
            minX = X;
        }
    }
//    qDebug() << "minX = " << minX;

    result = minX;
}
