#include "mainwindow.h"
#include "QtPanel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // Livewire::QtPanel qtTest;
    //qtTest.show();
    MainWindow m;
    m.setFixedHeight(600);
    m.setFixedWidth(150);
    m.move(100,80);
    m.show();
    return a.exec();
}


