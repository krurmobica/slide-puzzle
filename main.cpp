#include "renderarea.h"
#include "slider_mainwindow.h"
#include <QApplication>
#include <QRect>
#include "grid.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SliderMainWindow w;
    srand (time(0));
    w.show();

    return a.exec();
}
