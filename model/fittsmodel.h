#ifndef FITTSMODEL_H
#define FITTSMODEL_H

#include <QPoint>
#include <QList>
#include <QPolarChart>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStackedLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QLineSeries>
#include <QtCharts>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QChart>
#include <QGraphicsDropShadowEffect>
#include <QToolBar>
#include <QToolButton>
#include <QScrollArea>
#include <QDateTime>



class FittsView;
class FittsController;

class FittsModel
{
public:
    FittsModel();

private:
    FittsView *fittsView;

    int cibleLeft = 0;
    int nbCible = 10;
    int minSize = 10;
    int maxSize = 150;

    double a = 0.20;
    double b = 0.10;

    double ecartType = 0;
    double erreurType = 0;
    double diffMoy = 0;
    double itc95 = 0;

    QList<QPoint> clickPoints;
    QList<QPoint> cercleCenter;
    QList<int> cercleSize;
    QList<qint64> times;

    QChartView *plotHome;
    QChartView *plotHomeDistance;

    friend FittsView;
    friend FittsController;
};

#endif // FITTSMODEL_H
