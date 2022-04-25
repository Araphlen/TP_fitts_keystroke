#include "controller/fittscontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    a.setApplicationName("HM40 Fitts Test");

    new FittsController;

    return a.exec();
}
