#include "mainwindow.h"

#include <QApplication>
#include "login.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    //ButtonGroup w;
    //MainWindow w;
    w.show();
    return a.exec();
}
