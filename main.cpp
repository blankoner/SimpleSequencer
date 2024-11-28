#include "mainwindow.h"

#include <QApplication>
//#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("blank Sequencer v. 0.0.1");
    w.show();
    return a.exec();
}
