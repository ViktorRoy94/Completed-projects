#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "input_from_txt.h"
#include "input_generation.h"
#include "Lemke.h"
#include "Monte-Carlo.h"
#include "func.h"

const int zoom = 80;
const int rectSize = 2;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->QwtWidget->setTitle("Statistic");
    ui->QwtWidget->setAxisScale(QwtPlot::yLeft, 0, 5);
    ui->QwtWidget_2->setTitle("The timeline of the dimension of the method");
    ui->QwtWidget_2->setAxisScale(QwtPlot::xBottom, 0, 0.6);
    ui->QwtWidget_2->setAxisScale(QwtPlot::yLeft, 0, 100);
    ui->QwtWidget_2->setAxisTitle(QwtPlot::xBottom, "time, s");
    ui->QwtWidget_2->setAxisTitle(QwtPlot::yLeft, "N");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPolygonF findCoditionArea(const QVector <double> &A, const QVector <double> &b);
QPolygonF resizePolygon(const QPolygonF &polygon, const double width, const double height);

void MainWindow::on_startButton_clicked()
{
/*  минимизировать с^t * x + 1/2 x^t H x
    При условиях Ax <= b, x >= 0
    После преобразования:
                   Ax + y = b
        - Hx - (A^t)u + v = c
                  (x^t)*v = 0
                  (u^t)*y = 0
              x, y, u, v >= 0
    А - матрица m*n
    H - симметричная матрица n*n
    b - m-мерный вектор
    с - n-мерный вектор

    Получившуюся системы сводим к линейной задаче дополнительности, где
    M - матрица (m+n)*(m+n)
    q - вектор m+n
    w - вектор m+n
    z - вектор m+n
*/

    QString fileName = "d:/Projects/TEST_MO_coursework/input.txt";
    QVector <double> A;
    QVector <double> H;
    QVector <double> b;
    QVector <double> c;
    int errorCode; /*= inputFromTXT(fileName, A, H, b, c);
    if (errorCode != 0) {
        qDebug() << "ERROR(main.cpp): inputFromTXT";
        exit(0);
    }*/
    QVector <double> x_;
    double xMin = ui->xMinLineEdit->text().toDouble();
    double yMin = ui->yMinLineEdit->text().toDouble();
    x_ << xMin << yMin;
    errorCode = generInput(A, H, b, c, x_, 2, 2);
    if (errorCode != 0) {
        qDebug() << "ERROR(main): generInput";
        exit(0);
    }
    qDebug() << "H = " << H;
    qDebug() << "A = " << A;
    qDebug() << "b = " << b;
    qDebug() << "c = " << c;
    qDebug() << "x_ = " << x_;

    qDebug() << "minFunc = " << func(x_, H, c);

    QTime time;
    double lemkeTime;
    QVector <double> lemkeResult;
    time.start();
    lemke(A, H, b, c, lemkeResult);
    lemkeTime = (double)time.elapsed()/1000;
    qDebug() << "Lemke result: " << lemkeResult;
    qDebug() << "Lemke func result: " << func(lemkeResult, H, c);
    qDebug() << "Lemke time: " << lemkeTime << "s";


    time.start();
    QVector <double> monteCarloResult;
    double monteCarloTime;
    monteCarloMethod(A, H, b, c, rectSize, monteCarloResult, 10000);
    monteCarloTime = (double)time.elapsed()/1000;
    qDebug() << "Monte Carlo result: " << monteCarloResult;
    qDebug() << "Monte Carlo: " << monteCarloTime << "s";

    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    QPen penCOO(Qt::green);
    int height = ui->graphicsView->height();
    int width = ui->graphicsView->width();
    scene->addLine(0, height/2, width, height/2, penCOO);
    scene->addLine(width/2, 0, width/2, height, penCOO);
    ui->graphicsView->setScene(scene);

    QPen conditionAreaPen(Qt::gray);
    QBrush conditionAreaBrush(Qt::gray, Qt::BrushStyle::Dense6Pattern);
    QPolygonF area = findCoditionArea(A, b);
    area = resizePolygon(area, width, height);
    qDebug() << "Area = " << area;
    scene->addPolygon(area, conditionAreaPen, conditionAreaBrush);

    QPen solutionPen(Qt::red);
    QBrush solutionBrush(Qt::red);
    double x1 = lemkeResult[0]*zoom + width/2;
    double y1 = height/2 - lemkeResult[1]*zoom;
    scene->addEllipse(x1,y1,5,5,solutionPen, solutionBrush);

    QPen rectPen(Qt::blue);
    double xRect = width/2 - rectSize*zoom;
    double yRect = height/2 - rectSize*zoom;
    scene->addRect(xRect, yRect, 2*rectSize*zoom, 2*rectSize*zoom, rectPen, Qt::BrushStyle::NoBrush);

    QPen monteCarloPen(Qt::black);
    QBrush monteCarloBrush(Qt::black);
    double x2 = monteCarloResult[0]*zoom + width/2;
    double y2 = height/2 - monteCarloResult[1]*zoom;
    scene->addEllipse(x2,y2,5,5,monteCarloPen, monteCarloBrush);

    QPen minFuncPen(Qt::blue);
    QBrush minfuncBrush(Qt::blue, Qt::BrushStyle::Dense7Pattern);
    double xMin_ = xMin*zoom + width/2;
    double yMin_ = height/2 - yMin*zoom;
    scene->addEllipse(xMin_,yMin_,5,5,minFuncPen, minfuncBrush);

    return;
}

QPolygonF findCoditionArea(const QVector <double> &A, const QVector <double> &b) {
    QVector <QPointF> condition1;
    QVector <QPointF> condition2;
    QVector <QPointF> condition3;
    // x1 >= 0 x2 >= 0
    condition1 << QPointF(0,0) << QPointF(rectSize, 0)
               << QPointF(rectSize, rectSize) << QPointF(0, rectSize);
    // ограничение даваемое первой строкой матрицы
    if (A[0] < 0)
        condition2 << QPointF(0,0) << QPointF(0, b[0]/A[1])
                   << QPointF((b[0]-rectSize*A[1])/A[0], rectSize)
                   << QPointF(rectSize, 0);
    else
        condition2 << QPointF(0,0) << QPointF(b[0]/A[0], 0) << QPointF(0, b[0]/A[1]);
    // ограничение даваемое второй строкой матрицы
    if (A[2] < 0)
        condition3 << QPointF(0,0) << QPointF(0, b[1]/A[3])
                   << QPointF((b[1]-rectSize*A[3])/A[2], rectSize)
                   << QPointF(rectSize, 0);
    else
        condition3 << QPointF(0,0) << QPointF(b[1]/A[2], 0) << QPointF(0, b[1]/A[3]);
    qDebug() << "condition1 = " << condition1;
    qDebug() << "condition2 = " << condition2;
    qDebug() << "condition3 = " << condition3;
    QPolygonF area1(condition1);
    QPolygonF area2(condition2);
    QPolygonF area3(condition3);
    QPolygonF area = area1.intersected(area2.intersected(area3));
    return area;
}

QPolygonF resizePolygon(const QPolygonF &polygon, const double width, const double height) {
    QPolygonF result;
    for (int i = 0; i < polygon.size(); i++) {
        double x =  zoom*polygon.at(i).x() + width/2;
        double y = -zoom*polygon.at(i).y() + height/2;
        result << QPointF(x, y);
    }
    return result;
}


void MainWindow::on_calculateButton_clicked()
{
    int n = 2;
    int m = 2;
    QVector <double> A;
    QVector <double> H;
    QVector <double> b;
    QVector <double> c;
    QVector <double> x_(n);
//    QString fileName = "d:/Projects/TEST_MO_coursework/input.txt";
//    int errorCode = inputFromTXT(fileName, A, H, b, c);
//    if (errorCode != 0) {
//        qDebug() << "ERROR(main.cpp): inputFromTXT";
//        exit(0);
//    }

    for (int i = 0; i < n; i++)
        x_[i] = ((double) rand() / (RAND_MAX))+0.01;
//    x_ << 1 << 1 ;
    generInput(A, H, b, c, x_, n, m);

    qDebug() << "H = " << H;
    qDebug() << "A = " << A;
    qDebug() << "b = " << b;
    qDebug() << "c = " << c;
    qDebug() << "x_ = " << x_;

    qDebug() << "minFunc = " << func(x_, H, c);

    QTime time;
    double lemkeTime;
    double lemkeFuncResult;
    QVector <double> lemkeResult;
    time.start();
    lemke(A, H, b, c, lemkeResult);
    lemkeTime = (double)time.elapsed()/1000;
    lemkeFuncResult = func(lemkeResult, H, c);
//    qDebug() << "Lemke result: " << lemkeResult;
    qDebug() << "Lemke func result: " << lemkeFuncResult;
//    qDebug() << "Lemke time: " << lemkeTime << "s";


    time.start();
    QVector <double> monteCarloResult;
    QVector <double> monteCarloTime;
    QVector <double> monteCarloFunc;
    double sumFuncMonteCarlo = 0.0;
    int count = 0;
    int p = 100;
    while (abs(monteCarloFunc.last() - lemkeFuncResult) > 10e-2) {
        monteCarloMethod(A, H, b, c, 1.5, monteCarloResult, p);
        monteCarloTime << (double)time.elapsed()/1000;
        monteCarloFunc << func(monteCarloResult, H, c);
        sumFuncMonteCarlo += monteCarloFunc.last();
        count++;
//        qDebug() << "Monte Carlo result: " << monteCarloResult;
//        qDebug() << "MonteCarlo func result = " << monteCarloFunc.last();
//        qDebug() << "Monte Carlo: " << monteCarloTime << "s";
        p += 100;
    }
    double averageMonteCarlo = sumFuncMonteCarlo / count;
    QVector <double> ksi(count);
    QVector <double> teta(count);
    for (int i = 0; i < count; i++) {
        teta[i] = monteCarloTime[i] / lemkeTime;
        ksi[i] = abs(monteCarloFunc[i]-averageMonteCarlo) / abs(lemkeFuncResult - averageMonteCarlo);
    }
//    qDebug() << "count = " << count;
//    qDebug() << "monteCarloFunc = " << monteCarloFunc[11];
//    qDebug() << "teta = " << teta;
//    qDebug() << "ksi = " << ksi;

    ui->QwtWidget->setAxisScale(QwtPlot::xBottom, 0, (int)(teta.last()+1));

    QwtPlotCurve *curve = new QwtPlotCurve(QString("graph"));
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve->setPen(QPen(Qt::red));
    curve->setSamples(teta, ksi);
    curve->setStyle(QwtPlotCurve::Lines);
    curve->attach(ui->QwtWidget);
    ui->QwtWidget->replot();
    ui->QwtWidget->canvas()->setCursor(Qt::ArrowCursor);


 //   delete curve;
}

void MainWindow::on_calculateButton_2_clicked()
{

    QVector <double> lemkeTime;
    QVector <double> dimension;
    for (int iter = 2; iter < 65; iter++) {
        int n = iter;
        int m = iter;
        dimension << iter;
        QVector <double> A;
        QVector <double> H;
        QVector <double> b;
        QVector <double> c;
        QVector <double> x_(n);

        for (int i = 0; i < n; i++)
            x_[i] = ((double) rand() / (RAND_MAX))+0.01;

        generInput(A, H, b, c, x_, n, m);

//        qDebug() << "H = " << H;
//        qDebug() << "A = " << A;
//        qDebug() << "b = " << b;
//        qDebug() << "c = " << c;
//        qDebug() << "x_ = " << x_;

//        qDebug() << "minFunc = " << func(x_, H, c);


        QTime time;
        double lemkeFuncResult;
        QVector <double> lemkeResult;
        time.start();
        lemke(A, H, b, c, lemkeResult);
        lemkeTime << (double)time.elapsed()/1000;
        lemkeFuncResult = func(lemkeResult, H, c);
    //    qDebug() << "Lemke result: " << lemkeResult;
//        qDebug() << "Lemke func result: " << lemkeFuncResult;
    //    qDebug() << "Lemke time: " << lemkeTime << "s";

          qDebug() << "iter: " << iter;
    }

    QwtPlotCurve *curve = new QwtPlotCurve(QString("graph"));
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve->setPen(QPen(Qt::red));
    curve->setSamples(lemkeTime, dimension );
    curve->setStyle(QwtPlotCurve::Lines);
    curve->attach(ui->QwtWidget_2);
    ui->QwtWidget_2->replot();
    ui->QwtWidget_2->canvas()->setCursor(Qt::ArrowCursor);

}
