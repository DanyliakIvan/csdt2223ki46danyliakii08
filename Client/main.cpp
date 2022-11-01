#include "mainwindow.h"
#include "Lab_2.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CSDT csdt;
    csdt.resize(480, 350);
    csdt.setWindowTitle("Lab_2_CSDT_Danyliak");
    csdt.show();

    return a.exec();
}
