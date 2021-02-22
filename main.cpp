#include "mainwindow.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;
    MainWindow widget;
    window.setCentralWidget(&widget);
    window.resize(900, 600);
    window.show();
    return a.exec();
}
