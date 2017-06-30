#ifndef FUNC
#define FUNC

double func(const QVector<double> X, const QVector<double>& H, const QVector<double>& c) {
    int n = X.size();
    double result = 0.0;
    QVector <double> temp(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            temp[i] += H[i*n+j]*X[j];
        }
    }
    for (int i = 0; i < n; i++) {
        result += temp[i]*X[i];
    }
    result *= 0.5;
    for (int i = 0; i < n; i++) {
        result += c[i]*X[i];
    }
    return result;
}

#endif // FUNC

