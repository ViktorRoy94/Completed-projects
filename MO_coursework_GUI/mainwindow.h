#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qwt_plot_magnifier.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include <QString>
#include <QPolygon>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_calculateButton_clicked();

    void on_calculateButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
