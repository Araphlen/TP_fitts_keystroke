#include "fittscontroller.h"
#include "fittsview.h"
#include "fittsmodel.h"

#include <QApplication>
#include <QDebug>
#include <QtGlobal>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QHorizontalBarSeries>
#include <QCategoryAxis>
#include <QPieSeries>
#include <QPieSlice>
#include <QStringList>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QScatterSeries>

QT_CHARTS_USE_NAMESPACE

FittsController::FittsController() {
    this->fittsModel = new FittsModel();
    this->fittsView = new FittsView(this, this->fittsModel);
    //this->histModel = new QList<FittsModel>();
    this->start();
}

//changegraphhome
void FittsController::changeGraphHome(){
    if(this->fittsView->switchGraphHome->minimumHeight() == 250){
        this->fittsView->switchGraphHome->setIcon(QIcon(":/icons/icons/switchGraphe_2"));
        this->fittsView->switchGraphHome->setMinimumHeight(249);
        this->fittsView->plotHomeDistance->setVisible(true);
        this->fittsView->plotHome->setVisible(false);
        this->fittsView->graphTitleHome->setText("Temps en fonction de la distance");
}else{
    this->fittsView->switchGraphHome->setIcon(QIcon(":/icons/icons/switchGraphe_1"));
    this->fittsView->switchGraphHome->setMinimumHeight(250);
    this->fittsView->plotHomeDistance->setVisible(false);
    this->fittsView->plotHome->setVisible(true);
    this->fittsView->graphTitleHome->setText("Temps pour atteindre une cible");
}
}
//affiche la fenetre d'aide
void FittsController::aideClicked() {
    this->fittsView->displayAideDialog();
}

//lance l'affichage du programme 
void FittsController::start() {
    this->fittsView->show();
}

void FittsController::startSimulation() {
    if (this->fittsModel->minSize > this->fittsModel->maxSize) return;
    this->fittsView->mainStack->setCurrentIndex(2);
    this->fittsModel->cibleLeft = this->fittsModel->nbCible;
    this->fittsView->updateTestMsg();
    this->fittsView->graphicView->setEnabled(true);
    this->fittsModel->cercleSize.clear();
    this->fittsModel->cercleCenter.clear();
    this->fittsModel->clickPoints.clear();
    this->fittsModel->times.clear();

    this->initGame();
}

void FittsController::quit() {
    QApplication::quit();
}

void FittsController::backToMenu() {
    this->fittsView->mainStack->setCurrentIndex(0);
}

void FittsController::backToSettings() {
    this->fittsView->mainStack->setCurrentIndex(1);
}
void FittsController::resultClicked() {
    this->fittsView->mainStack->setCurrentIndex(3);

    this->calculateResult();
}
void FittsController::aValueChanged(double value) {
    this->fittsModel->a = value;
}
void FittsController::bValueChanged(double value) {
    this->fittsModel->b = value;
}
void FittsController::nbCibleChanged(int value) {
    this->fittsModel->nbCible = value;
}
void FittsController::minSizeChanged(int value) {
    this->fittsModel->minSize = value;
    if (this->fittsModel->minSize > this->fittsModel->maxSize){
        QMessageBox::critical(this->fittsView, "Attention","La taille minimale est supérieur à la taille max");
        this->fittsView->homeStartBtn->setDisabled("true");
    }
    else{
        this->fittsView->homeStartBtn->setEnabled("true");
    }
}
void FittsController::maxSizeChanged(int value) {
    this->fittsModel->maxSize = value;
    if (this->fittsModel->minSize > this->fittsModel->maxSize){
        QMessageBox::critical(this->fittsView, "Attention","La taille minimale est supérieur à la taille max");
        this->fittsView->homeStartBtn->setDisabled("true");
    }

    else{
        this->fittsView->homeStartBtn->setEnabled("true");
    }

}
void FittsController::cibleClicked(int x, int y) {
    if(this->fittsModel->cercleCenter.isEmpty()) {
        // Si vide alors premier click, on demarre le timer
        this->timer = new QElapsedTimer;
        timer->start();

        // On démarre avec la première cible
        this->fittsModel->clickPoints.append(QPoint(x,y));
        this->nextCible();
    }
    else {
        QPointF coords = this->fittsView->graphicView->mapToScene(x,y);
        if(sqrt(pow(coords.x() - this->fittsModel->cercleCenter.last().x(),2) + pow(coords.y() - this->fittsModel->cercleCenter.last().y(),2)) <= this->fittsModel->cercleSize.last() / 2) {
            // On stock le temps de click
            this->fittsModel->times.append(timer->elapsed());
            // On restart le chrono
            timer->restart();

            // On stock la position du click
            this->fittsModel->clickPoints.append(QPoint(x,y));
            this->nextCible();
        }
    }
}

void FittsController::nextCible() {
    if(!this->fittsModel->cercleCenter.isEmpty())
        this->fittsModel->cibleLeft--;
    this->fittsView->updateTestMsg();

    QGraphicsScene *scene = this->fittsView->scene;
    scene->clear();

    // On stop si c'est finis
    if(this->fittsModel->cibleLeft == 0) {
        this->finish();
        return;
    }

    // On génère la taille du cercle rouge
    // qrand() % ((high + 1) - low) + low;
    int size = qrand() % ((this->fittsModel->maxSize + 1) - this->fittsModel->minSize) + this->fittsModel->minSize;

    // Car on veut le rayon
    // On place le cercle dans la scene (Attention faut pas qu'il soit en dehors du cadre)
    int sceneW = int(this->fittsView->scene->width());
    int sceneH = int(this->fittsView->scene->height());

    qreal posX = qrand() % ((sceneW - size) - size) + size;
    qreal posY = qrand() % ((sceneH - size) - size) + size;

    // On stock les infos sur le cercle
    this->fittsModel->cercleCenter.append(QPoint(int(posX),int(posY)));
    this->fittsModel->cercleSize.append(size);

    // On place le cercle
    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor("#20B773")),QBrush(QColor("#20B773")));
}


void FittsController::finish() {
    this->fittsView->graphicView->setEnabled(false);
    this->resultClicked();
}

void FittsController::initGame() {
    QGraphicsScene *scene = this->fittsView->scene;
    scene->clear();

    if(this->fittsModel->maxSize >= this->fittsView->graphicView->width() / 2)
        this->fittsModel->maxSize = this->fittsView->graphicView->width() / 2;

    if(this->fittsModel->maxSize >= this->fittsView->graphicView->height() / 2)
        this->fittsModel->maxSize = this->fittsView->graphicView->height() / 2;

    qreal posX = scene->width() / 2;
    qreal posY = scene->height() / 2;
    int size = 100;

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(30);
    effect->setXOffset(0);
    effect->setYOffset(0);
    effect->setColor(color_blue);
    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor(color_blue)),QBrush(QColor(color_blue)))->setGraphicsEffect(effect);
    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor(color_blue)),QBrush(QColor(color_blue)))->setCursor(Qt::PointingHandCursor);
    QLabel *labelStart;
    labelStart = new QLabel("Start");
    labelStart->setAlignment(Qt::AlignCenter);
    labelStart->setStyleSheet("color: #ffffff; font: bold 35px 'ROBOTO'; background-color: transparent;");
    labelStart->setCursor(Qt::PointingHandCursor);
    labelStart->move((size / 2) + 227, (size / 2) + 75);
    scene->addWidget(labelStart);
}

void FittsController::calculateResult() {

    //chart 1



// New PlotHome
    QChart *chartHome = new QChart;
    this->fittsView->plotHome->setChart(chartHome);
    this->fittsView->plotHome->setRenderHint(QPainter::Antialiasing);
    chartHome->setAnimationOptions(QChart::AllAnimations);
    chartHome->createDefaultAxes();
    chartHome->legend()->setVisible(false);
    chartHome->legend()->setLabelBrush(QBrush(QColor(color_white)));
    chartHome->setBackgroundVisible(false);

    QLineSeries *expSeries = new QLineSeries;
    QLineSeries *fittsSeries = new QLineSeries;
    QCategoryAxis *axis = new QCategoryAxis;

    expSeries->setName("Données expérimentales");
    expSeries->setColor("red");
    fittsSeries->setName("Courbe théorique");
    fittsSeries->setColor("green");

    //New plotHomeDistance
    QChart *chartDistanceHome = new QChart;
    this->fittsView->plotHomeDistance->setChart(chartDistanceHome);
    this->fittsView->plotHomeDistance->setRenderHint(QPainter::Antialiasing);
    chartDistanceHome->setAnimationOptions(QChart::AllAnimations);
    chartDistanceHome->createDefaultAxes();
    chartDistanceHome->legend()->setVisible(false);
    chartDistanceHome->legend()->setLabelBrush(QBrush(QColor(color_white)));
    chartDistanceHome->setBackgroundVisible(false);

    QScatterSeries *expSeriesDistance = new QScatterSeries;
    QLineSeries *fittsSeriesDistance = new QLineSeries;
    QCategoryAxis *axisDistance = new QCategoryAxis;


    expSeriesDistance->setName("Données expérimentales");
    expSeriesDistance->setColor("red");
    expSeriesDistance->setMarkerSize(11);
    fittsSeriesDistance->setName("Courbe théorique");
    fittsSeriesDistance->setColor("green");


    QList<double> fittsValues;
    QList<double> fittsValues2;

    for(int i = 0; i < this->fittsModel->nbCible; ++i) {
        double T = this->fittsModel->times[i];
        expSeries->append(i,T);
        double D = sqrt(pow(this->fittsModel->clickPoints[i].x() - this->fittsModel->cercleCenter[i].x(),2) + pow(this->fittsModel->clickPoints[i].y() - this->fittsModel->cercleCenter[i].y(),2));
        double L = this->fittsModel->cercleSize[i];
        double F = log2(2*D/L);
        expSeriesDistance->append(F,T);

        // On multiplie par 100 pour être en ms
        double value = (this->fittsModel->a * 1000) + ((this->fittsModel->b * 1000) * log2((D / this->fittsModel->cercleSize[i]) + 1));
        fittsValues.append(value);
        fittsSeries->append(i,value);
        fittsSeriesDistance->append(F,value);
        axis->append(QString::number(i + 1), i);
        axisDistance->append(QString::number(F),i);

    }

    QVector<QPointF> points = fittsSeriesDistance->pointsVector();
    std::sort(points.begin(), points.end(), [](const QPointF & p1, const QPointF & p2) {
        return p1.y() > p2.y();
    });
    fittsSeriesDistance->replace(points);

    QVector<QPointF> points2 = expSeriesDistance->pointsVector();
    std::sort(points2.begin(), points2.end(), [](const QPointF & p21, const QPointF & p22) {
        return p21.y() > p22.y();
    });
    expSeriesDistance->replace(points2);

    axis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisDistance->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    chartHome->addSeries(expSeries);
    chartHome->addSeries(fittsSeries);
    chartDistanceHome->addSeries(expSeriesDistance);
    chartDistanceHome->addSeries(fittsSeriesDistance);

    chartHome->setAxisX(axis,expSeries);
    chartHome->setAxisX(axis,fittsSeries);
    chartDistanceHome->setAxisX(axisDistance,expSeriesDistance);
    chartDistanceHome->setAxisX(axisDistance,fittsSeriesDistance);

    QValueAxis *axisY = new QValueAxis;
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY2 = new QValueAxis;
    QValueAxis *axisX2 = new QValueAxis;
    axisY->setTitleText("temps (en ms)");
    axisY2->setTitleText("temps (en ms)");
    axisX->setTitleText("Numéro de la cible");
    axisX2->setTitleText("log(2D/L)");
    chartHome->setAxisY(axisY,expSeries);
    chartHome->setAxisX(axisX,expSeries);
    chartDistanceHome->setAxisY(axisY2,expSeriesDistance);
    chartDistanceHome->setAxisX(axisX2,expSeriesDistance);

    // Calcul des valeurs
    // Moyennes
    QList<double> diffValues;
    double diffMoy = 0;

    for (int i = 0; i < fittsValues.size(); ++i) {
        diffValues.append(fabs(fittsValues[i] - this->fittsModel->times[i]));
        diffMoy += fabs(fittsValues[i] - this->fittsModel->times[i]);
    }
    diffMoy /= fittsValues.size();

    // On stock la difference de moyenne
    this->fittsModel->diffMoy = fabs(diffMoy);

    // Ecart type
    double variance = 0;

    for (int i = 0; i < fittsValues.size(); ++i) {
        variance += pow(diffValues[i] - diffMoy,2);
    }
    variance /= fittsValues.size();

    double ecartType = sqrt(variance);
    // On stock l'ecart type
    this->fittsModel->ecartType = ecartType;
    // On stock l'erreur type
    this->fittsModel->erreurType = fabs(ecartType / sqrt(fittsValues.size()));

    // On stock itc 95%
    this->fittsModel->itc95 = 2 * this->fittsModel->erreurType;

    this->fittsView->displayResults();
}
