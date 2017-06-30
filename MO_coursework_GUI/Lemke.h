#include <QVector>
#include <test.h>

// Алгоритм Лемке для линейной задачи дополнительности
int lemke_algorithm(const QVector<double> &M, const QVector<double> &q, QVector<double> &w, QVector<double> &z);

// Вспомогательная функция, которая элементарными преобразованиями
// очищает столбец над и под (i_,j_) элементом
int iteration(QVector<double> &A, int n, int i_, int j_);

// переходим от задачи квадратичного программирования к линейное задачи дополнительности,
// M, q - матрица и вектор в задачи дополнительности
void lemke(const QVector<double> &A, const QVector<double> &H, const QVector<double> &b,
    const QVector<double> &c, QVector<double> &result) {
    int n = c.length();
    int m = b.length();
    int N = m + n;

    QVector <double> M(N*N);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++)
            M[i*N+j] = 0;
        for (int j = m; j < N; j++)
            M[i*N+j] = -A[i*n+j-m];
    }

    for (int i = m; i < N; i++) {
        for (int j = 0; j < m; j++)
            M[i*N+j] = A[j*n+i-m];
        for (int j = m; j < N; j++)
            M[i*N+j] = H[(i-m)*n + j-m];
    }

    QVector <double> q(N);
    for (int i = 0; i < m; i++)
        q[i] = b[i];
    for (int i = m; i < N; i++)
        q[i] = c[i-m];

//    printTest("d:/Projects/TEST_MO_coursework/M_test.txt", M, N, 0);
//    printTest("d:/Projects/TEST_MO_coursework/q_test.txt", q, 0, 0);

//    qDebug() << "M = " << M;
    QVector <double> w;
    QVector <double> z;
//    qDebug() << "M = " << M;
//    qDebug() << "q = " << q;
    lemke_algorithm(M, q, w, z);
//    qDebug() << "w = " << w;
//    qDebug() << "z = " << z;
    result.fill(0, n);
    for (int i = 0; i < n; i++) {
        result[i] = z[z.size()-n+i];
    }
}

int lemke_algorithm(const QVector<double> &M, const QVector<double> &q, QVector<double> &w, QVector<double> &z) {
    int n = sqrt(M.length());
//    qDebug() << "n = " << n;
    int m = n+n+2;
    w.resize(n);
    z.resize(n);

    // проверка на очевидное решение
    int flag = 0;
    for (int i = 0; i < n; i++) {
        if (q[i] < 0)
            flag = 1;
    }
    if (!flag) {
        w = q;
        z.fill(0);
        return 0;
    }

    // таблица, которую будем преобразовывать в ходе алгоритма
    // 0.. n-1 - вектора перед w1-wn
    // n..2n-1 - вектора перед z1-zn
    // 2n      - вектор перед искусственной переменной z0
    // 2n+1    - правая часть (q)

    QVector <double> table (n*m);
    // w1..wn
    for (int i = 0; i < n; i++) {
        table[i*m+i] = 1;
    }
    // М
    for (int i = 0; i < n; i++) {
        for (int j = n; j < 2*n; j++) {
            table[i*m+j] = -M[i*n+j-n];
        }
    }
    // z1..zn
    for (int i = 0; i < n; i++) {
        table[i*m+2*n] = -1;
    }
    // правая часть
    for (int i = 0; i < n; i++) {
        table[i*m+2*n+1] = q[i];
    }
//    printTest("d:/Projects/TEST_MO_coursework/lemke_algorithm.txt", table, n, 0);
//    printTest("d:/Projects/TEST_MO_coursework/table.txt", table, n, 0);

    // массив, в котором хранятся базисные переменные. 0-n это w1-wn,n-2n это z1-zn и z0
    QVector <int> basis(n);
    for (int i = 0; i < n; i++)
        basis[i] = i;
    int leaving = 0; // переменная покидающая базис
    int entering = 0; // переменная вступающая базис

    // начальный этап
    int s  = 0;
    double min = 1000000;
    for (int i = 0; i < n; i++) {
        if (q[i] < min) {
            min = q[i];
            s = i;
        }
    }
    min = 1000000;
    entering = n + s; // entering = zs
    iteration(table, n, s, 2*n);
    basis[s] = 2*n; // z0 в базис

//    qDebug() << "basis = " << basis;
//    printTest("d:/Projects/TEST_MO_coursework/lemke_algorithm.txt", table, n, 1);

    // Основной этап
    while(true) {
        // проверка на луч ds <=0
        int rayFlag = 0;
        for (int i = 0; i < n; i++){
            if (table[i*m+entering] > 0)
                rayFlag = 1;
        }
        if (!rayFlag) {
            // ПЕРЕХОД К 4 ШАГУ
            qDebug() << "find ray";
            return 0;
        }

        int r = 0;
        for (int i = 0; i < n; i++){
            if (table[i*m+entering] > 0){
                double temp = table[i*m+2*n+1]/table[i*m+entering];
//                qDebug() << "i = " << i << " temp = " << temp << " min = " << min;
                if (temp < min) {
                    min = temp;
                    r = i;
                }
            }
        }
        min = 1000000;
        if (basis[r] == 2*n) {
            iteration(table, n, r, entering);
            basis[r] = entering;
//            qDebug() << "basis = " << basis;
//            printTest("d:/Projects/TEST_MO_coursework/lemke_algorithm.txt", table, n, 1);
            // сохраняем результаты
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < n; i++){
                    if (table[i*m+j] == 1)
                        w[j] = table[i*m+2*n+1];
                }
            }
            for (int j = n; j < 2*n; j++) {
                for (int i = 0; i < n; i++){
                    if (table[i*m+j] == 1)
                        z[j-n] = table[i*m+2*n+1];
                }
            }
            return 0;
        }

        iteration(table, n, r, entering);

        leaving  = basis[r];
        basis[r] = entering;
//        qDebug() << "basis = " << basis;
//        printTest("d:/Projects/TEST_MO_coursework/lemke_algorithm.txt", table, n, 1);

        if (leaving >= 0 && leaving <= n-1) {
            entering = n + leaving;
        }
        if (leaving >= n && leaving <= 2*n-1) {
            entering = leaving - n;
        }
//        qDebug() << "basis" << basis;
    }

    return 0;
}

int iteration(QVector<double> &A, int n, int i_, int j_) {
    int m = A.length() / n;
    double element = A[i_*m + j_];
    for (int i = 0; i < n; i++){
        double temp = A[i*m+j_];
        for (int j = 0; j < m; j++) {
            if (i != i_) {
                A[i*m+j] = A[i*m+j] - (A[i_*m+j] * temp) / element ;
                if ((A[i*m+j] > 1.0e-20 && A[i*m+j] < 1.0e-7) || A[i*m+j] < -1.0e-17 && A[i*m+j] > -1.0e-7)
                    A[i*m+j] = 0;
            }
        }
    }
    for (int j = 0; j < m; j++) {
        A[i_*m+j] = A[i_*m+j] / element;
    }

    return 0;
}

