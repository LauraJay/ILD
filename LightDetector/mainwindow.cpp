#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

using namespace cv;

QImage image;
std::vector<Contour*> Contours; //Alle fertigen Konturen
std::vector<Point2f> contourPoints; //Punkte während der Erstellung einer Kontur

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
}

void MainWindow::on_btm_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.png, *jpg, *.jpeg)"));

    if (QString::compare(filename, QString()) !=0){
        bool valid = image.load(filename);
        if (valid){
            image=image.scaledToWidth(ui->lbl_image->width(),Qt::SmoothTransformation);
            ui->lbl_image->setPixmap(QPixmap::fromImage(image)); //Load and Show image
            showSeg();
            ui->btm_EndSeg->hide();
            ui->btm_image->hide();
            ui->btm_restart->show();
        }
    }
}

//Can be used to start a new conture
void MainWindow::on_btm_StartSeg_clicked()
{
    //TO DO Laura:
   //Klicken
   //Punkt auf Vektor pushen
   //ab zwei Punkten im Vektor Seg Algo aufrufen
    //Wenn genug Punkte in Vektor, siehe unten:

    bool EnoughPoints = true;
   // if(contourPoints.size()>= 3){
    if(EnoughPoints){
    ui->btm_EndSingleSeg->show();
    }

}

void MainWindow::on_btm_EndSingleSeg_clicked()
{
    ui->btm_EndSingleSeg->hide();
    ui->btm_StartSeg->show();
    ui->btm_EndSeg->show();
    ui->btm_ChooseSeg->show();
    //Fertige Contour auf Vektor mit Konturen pushen
}

void MainWindow::on_btm_EndSeg_clicked()
{
    //Malmodus beenden
     hideSeg();
     showVisual();
     ui->btm_backToSeg->show();
}

void MainWindow::on_btm_restart_clicked()
{
    ui->btm_image->show();
    hideSeg();
    hideVisual();
    ui->btm_restart->hide();
    ui->btm_backToSeg->hide();
    ui->lbl_image->clear();
}

void MainWindow::on_btm_backToSeg_clicked()
{
    hideVisual();
    ui->btm_backToSeg->hide();
    ui->label->show();
    ui->btm_ChooseSeg->show();
    ui->btm_StartSeg->show();
    ui->btm_EndSeg->show();
}

void MainWindow::on_btm_ChooseSeg_clicked()
{
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
    ui->label->hide();
    ui->btm_StartSeg->hide();
    ui->btm_EndSeg->hide();
    ui->btm_ChooseSeg->hide();
    ui->btm_DeleteConture->hide();
    ui->btm_EndSingleSeg->hide();
}

void MainWindow::showSeg(){
    ui->label->show();
    ui->btm_StartSeg->show();
}

void MainWindow::hideVisual(){
    ui->label_2->hide();
    ui->btm_ShowSeg->hide();
    ui->btm_ShowLV->hide();
    ui->btm_ShowAreas->hide();
}

void MainWindow::showVisual(){
    ui->label_2->show();
    ui->btm_ShowSeg->show();
    ui->btm_ShowLV->show();
    ui->btm_ShowAreas->show();
}








