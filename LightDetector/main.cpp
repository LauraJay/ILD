#include "mainwindow.h"
#include "QtPanel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // Livewire::QtPanel qtTest;
    //qtTest.show();
    MainWindow m;
    m.show();
    return a.exec();
}


