#include <QtGui/QApplication>
#include "mainwindow.h"
#include "../HIDAPI/hidapi.h"



int main(int argc, char *argv[])
{
    hid_init();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int r = a.exec();
    hid_exit();
    return r;
}
