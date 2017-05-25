#include "mainwindow.h"
#include "QtPanel.h"
#include "QtGui"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

using namespace cv;

QImage image;
//std::vector<Contour*> Contours; //Alle fertigen Konturen
//std::vector<Point2f> contourPoints; //Punkte während der Erstellung einer Kontur


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hideSeg();
    hideVisual();
    ui->btm_restart->hide();
    ui->btm_backToSeg->hide();
}



MainWindow::~MainWindow()
{
    delete ui;
    delete qtp;
}

void MainWindow::on_btm_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*jpg,*jpeg)"));
    if (QString::compare(filename, QString()) !=0){
        bool valid = image.load(filename);
        if (valid){
            openQtPanel(image);
            ui->centralWidget->activateWindow();
            showSeg();
            ui->btm_EndSeg->hide();
            ui->btm_image->hide();
            ui->label_1->hide();
            ui->btm_restart->show();
            qtp->setMouseEvent(false); //It is not possible to detect Contours.
        }
    }
}




void MainWindow::openQtPanel(QImage img){
    qtp = new Livewire::QtPanel();
    qtp->show();


    if(img.height() <= ui->centralWidget->height()){
        qtp->setFixedHeight(img.height());
    } //scale to the maximum height
    else if(img.height() > ui->centralWidget->height()){
        qtp->setFixedHeight(ui->centralWidget->height()+32);
        qtp->setFixedWidth((img.width()+32)*ui->centralWidget->height()/img.height());
        img = img.scaledToHeight(centralWidget()->height()+32);
    }
    qtp->SetImage(img);
    qtp->move(260,80);
}

void MainWindow::on_btm_StartSeg_clicked()
{
    //Segmentation is switched on
    qtp->setMouseTracking(true);
    qtp->setMouseEvent(true);

    //TO DO: Länge der Kontur abfragen und entscheiden,
    //ob diese schon beendet werden kann.

    bool EnoughPoints = true;
    if(EnoughPoints){
    ui->btm_EndSingleSeg->show();
    }

}

void MainWindow::on_btm_EndSingleSeg_clicked()
{
    qtp->setMouseTracking(false);
    qtp->setMouseEvent(false);
    ui->btm_EndSingleSeg->hide();
    ui->btm_StartSeg->show();
    ui->btm_EndSeg->show();
    ui->btm_ChooseSeg->show();
    //TO DO 1: Fertige Contour auf Vektor mit Konturen pushen
    //TO DO 2: Gemalte Kontur aus Fenster löschen

}

void MainWindow::on_btm_EndSeg_clicked()
{
     hideSeg();
     showVisual();
     ui->btm_backToSeg->show();
}

void MainWindow::on_btm_restart_clicked()
{
    ui->btm_image->show();
    ui->label_1->show();
    hideSeg();
    hideVisual();
    ui->btm_restart->hide();
    ui->btm_backToSeg->hide();
    QImage blank = QImage();
    qtp->SetImage(blank);
}

void MainWindow::on_btm_backToSeg_clicked()
{
    hideVisual();
    ui->btm_backToSeg->hide();
    ui->label_2->show();
    ui->btm_ChooseSeg->show();
    ui->btm_StartSeg->show();
    ui->btm_EndSeg->show();
}

void MainWindow::on_btm_ChooseSeg_clicked()
{
    //TO DO: Rausfinden, wie man auf Konturen zugreift
   //Geignet alle vorhanden Konturen des Vektors Contours daretellen
    //Möglichkeit bieten einzelne Konturen auuszuwählen
}

void MainWindow::on_btm_DeleteConture_clicked()
{
    //Konturen aus Vektor löschen
    //Wenn keine Konturen mehr vorhanden sind, nur noch den Button zum neue Kontur erstellen anzeigen
}

void MainWindow::on_btm_ShowSeg_clicked()
{
    //show Segmentation
}

void MainWindow::on_btm_ShowLV_clicked()
{
    //Show Lightvectors
}

void MainWindow::on_btm_ShowAreas_clicked()
{
    //ShowAreas
}

void MainWindow::hideSeg(){
    ui->label_2->hide();
    ui->btm_StartSeg->hide();
    ui->btm_EndSeg->hide();
    ui->btm_ChooseSeg->hide();
    ui->btm_DeleteConture->hide();
    ui->btm_EndSingleSeg->hide();
}

void MainWindow::showSeg(){
    ui->label_2->show();
    ui->btm_StartSeg->show();
}

void MainWindow::hideVisual(){
    ui->label_3->hide();
    ui->btm_ShowSeg->hide();
    ui->btm_ShowLV->hide();
    ui->btm_ShowAreas->hide();
}

void MainWindow::showVisual(){
    ui->label_3->show();
    ui->btm_ShowSeg->show();
    ui->btm_ShowLV->show();
    ui->btm_ShowAreas->show();
}








