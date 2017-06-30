#ifndef TEST_H
#define TEST_H

#include <QFile>
#include <QVector>
#include <QString>
#include <QTextStream>

void printTest(const QString filePath, const QVector <double> &A, const int n, int append) {
    QFile file(filePath);
    QTextStream out(&file);
    if (append) {
        if (!file.open(QIODevice::Append)) {
            qDebug() << "ERROR(test.h): open file error";
        }
    }
    else {
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "ERROR(test.h): open file error";
        }
    }
    out.setFieldWidth(3);
    out.setRealNumberNotation(QTextStream::ScientificNotation);
    out.setFieldAlignment(QTextStream::AlignRight);

    if (n > 0) {
        // print Matrix
        int m = A.length() / n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                out << A[i*m+j] << " ";
            }
            out << endl;
        }
    }
    else
    {
        // print Vector
        for (int i = 0; i < A.length(); i++) {
            out << A[i] << " ";
            out << endl;
        }
    }
    out << endl;
}

#endif // TEST_H
