#include <QVector>
#include <QFile>
#include <QString>
#include <QDebug>

int inputFromTXT(QString fileName, QVector<double>& A, QVector<double>& H, QVector<double>& b, QVector<double>& c) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "ERROR(input_from_txt.h): file open error";
        return 1;
    }

    QTextStream in(&file);
    QString str = in.readLine();

    while(str != "A") {
        str = in.readLine();
    }
    str = in.readLine();
    QStringList temp = str.split(" ");
    for (int i = 0; i < temp.length(); i++) {
        A <<  temp[i].toInt();
    }

    while(str != "H") {
        str = in.readLine();
    }
    str = in.readLine();
    temp = str.split(" ");
    for (int i = 0; i < temp.length(); i++) {
        H <<  temp[i].toInt();
    }

    while(str != "b") {
        str = in.readLine();
    }
    str = in.readLine();
    temp = str.split(" ");
    for (int i = 0; i < temp.length(); i++) {
        b <<  temp[i].toInt();
    }

    while(str != "c") {
        str = in.readLine();
    }
    str = in.readLine();
    temp = str.split(" ");
    for (int i = 0; i < temp.length(); i++) {
        c <<  temp[i].toInt();
    }

	return 0;
}
