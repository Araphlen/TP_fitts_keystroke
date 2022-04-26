#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicwidget.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStackedLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QLineSeries>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

class FittsModel;
class FittsController;

class FittsView : public QMainWindow
{
    Q_OBJECT
public:
    FittsView(FittsController *fittsController, FittsModel *fittsModel);
    ~FittsView();

    void initWindows();
    void displayOptionDialog();
    void displayAideDialog();
private:
    void updateTestMsg();
    void displayResults();


    FittsModel *fittsModel;
    FittsController *fittsController;

    QPushButton *fittshomeBtn;
    QPushButton *keyhomeBtn;
    QPushButton *homeLeaveBtn;
    QPushButton *homeStartBtn;
    QPushButton *testHomeBtn;
    QPushButton *testRestartBtn;
    QPushButton *resultHomeBtn;
    QPushButton *resultRestartBtn;

    QDoubleSpinBox *aValue;
    QDoubleSpinBox *bValue;
    QSpinBox *nbCible;
    QSpinBox *minSize;
    QSpinBox *maxSize;

    QToolButton *switchGraphHome;

    QStackedLayout *mainStack;

    GraphicWidget *graphicView;
    QGraphicsScene *scene;

    QLabel *testLabel;
    QLabel *ecartType;
    QLabel *erreurType;
    QLabel *diffMoy;
    QLabel *itc95;
    QLabel *graphTitleHome;

    QMenu *menu_fichier;

    QAction *actionStart;
    QAction *actionQuitter;
    QAction *actionParametres;
    QAction *actionAide;

    QDialog *optionDialog;

    QChartView *plotHome;
    QChartView *plotHomeDistance;



    friend FittsController;




};

//Utiles
static QString color_bg = "#212121";
static QString color_black = "#121212";
static QString color_dark_grey = "#272727";
static QString color_light_grey = "#b7b7b7";
static QString color_blue = "#00dac7";
static QString color_blue_focus = "#00fcdd";
static QString color_purple = "#bd88f7";
static QString color_purple_focus = "#c49fff";
static QString color_red = "#d36579";
static QString color_red_focus = "#f77994";
static QString color_white = "#ffffff";
static QString color_grid = "#3d3f44";

static QString color_itemList_bg = "#151618";
static QString color_itemList_bg_focus = "#212121";

static QString button_radius = "15px";

#endif // MAINWINDOW_H
