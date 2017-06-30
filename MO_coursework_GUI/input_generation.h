#include <QVector>
#include <QTime>
#include <ctime>
#include "test.h"

// генерация тестовой задачи вида (x-x*)^(t)H(x-x*)-x*^(t)Hx*
// где x* - точка минимума функции

int generInput(QVector<double>& A, QVector<double>& H, QVector<double>& b,
               QVector<double>& c, const QVector<double> &x_, const int n, const int m) {
    srand(time(NULL));
    // генерация матрицы H = L^(t)DL
    // D - диагональная, L - верхне-треугольная
    QVector<double> L(n*n);
    QVector<double> D(n*n);
    D.fill(0);
    L.fill(0);
    for (int i = 0; i < n; i++) {
        D[i*n+i] = rand()%4 + 2;
        L[i*n+i] = 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            L[i*n+j] = rand()%5 + 2;
        }
    }

    QVector<double> temp(n*n);
    temp.fill(0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i*n+j] += D[i*n+i]*L[i*n+j];
        }
    }

    H.fill(0, n*n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for( int x = 0; x < n; x++){
                H[i*n+j] += L[x*n+i]*temp[x*n+j];
            }
        }
    }
//    qDebug() << "L = " << L;
//    qDebug() << "D = " << D;
//    printTest("d:/Projects/TEST_MO_coursework/D_test.txt", D, n, 0);
//    printTest("d:/Projects/TEST_MO_coursework/L_test.txt", L, n, 0);
//    printTest("d:/Projects/TEST_MO_coursework/temp_test.txt", temp, n, 0);
    printTest("d:/Projects/TEST_MO_coursework/H_test.txt", H, n, 0);

    // генерация вектора с
    c.fill(0, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i] += x_[j]*H[j*n+i];
        }
        c[i] = -c[i];
    }
    printTest("d:/Projects/TEST_MO_coursework/c_test.txt", c, 0, 0);


    // проверка на то, в какой области точка min функции
    QVector <double> x(x_);
    int zeroFlag = 1;
    int negFlag = 1;
    int power = 2;
    for (int i = 0; i < n; i++) {
        if (x_[i] != 0)
            zeroFlag = 0;
        if (x_[i] > 0){
            negFlag = 0;
        }
    }
    if (zeroFlag) {
        for(int i = 0; i < n; i++) {
            x[i] += 0.1;
        }
    }
    if (negFlag) {
        power = rand();
    }

    // генерация матрицы A и вектора b
    b.fill(0,m);
    A.fill(0,n*m);
    for (int i = 0; i < m; i++) {
        double temp = 0.0;
        b[i] = 1;
        while (temp <= b[i]) {
            double size = 0.0;
            double length = 0.0;
            int flag = 0;
            for (int j = 0; j < n; j++) {
                length += x[j]*x[j];
                if (x[j] < 0)
                    flag = 1;
            }
            if (flag)
                size = ((double) rand() / (RAND_MAX))*3+0.2;
            else
                size = sqrt(length);
            b[i] = ((double) rand() / (RAND_MAX))*size+0.2;
//            qDebug() << "sqrt(length) = " << sqrt(length);

            for (int j = 0; j < n; j++) {
                A[i*m+j] = pow(-1, 2)*((double) rand() / (RAND_MAX))*2 + 0.01;
            }
            temp = 0;
            for (int j = 0; j < n; j++) {
                temp += A[i*m+j]*x[j];
            }
//            qDebug() << "i = " << i;
//            qDebug() << "b[i] = " << b[i];
//            qDebug() << "temp = " << temp;
        }
    }
//    qDebug() << "A = " << A;
//    qDebug() << "b = " << b;


    return 0;
}
