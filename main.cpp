#include "renderarea.h"
#include "slider_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SliderMainWindow w;

    w.show();

    return a.exec();
}
