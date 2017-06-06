#include "mainwindow.h"
#include "QtPanel.h"
#include <QApplication>
#include"lightcalculator.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Livewire::QtPanel qtTest;
    //qtTest.show();
    MainWindow m;
    m.setFixedHeight(600);
    m.setFixedWidth(1100);
    m.move(100,80);
    m.show();
    LightCalculator* lc = new LightCalculator();
    return a.exec();
}


