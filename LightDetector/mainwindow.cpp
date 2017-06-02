#include "mainwindow.h"
#include "QtPanel.h"
#include "QtGui"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

using namespace cv;

QImage image;
QImage maskImage;
QVector<Contour*> Contours; //Alle fertigen Konturen
QVector<QPoint> contourPoints; //Punkte während der Erstellung einer Kontur


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hideVisual();
   }



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btm_mask_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose the Image you want to analyse"), "Testbilder",
            tr("Images (*.jpg)"));
    if (QString::compare(filename, QString()) !=0){
        bool valid = maskImage.load(filename);
        if (valid){
            maskImage = maskImage.scaledToWidth(ui->lbl_mask->width(),Qt::SmoothTransformation);
            ui->lbl_mask->setPixmap(QPixmap::fromImage(maskImage)); //Load and Show image
            ui->centralWidget->activateWindow();
            ui->btm_restart->show();
            ui->lbl_mask->show();
            ui->lbl_image->hide();
        }
    }
}

void MainWindow::on_btm_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose the Image you want to analyse"), "Testbilder",
            tr("Images (*.jpg)"));
    if (QString::compare(filename, QString()) !=0){
        bool valid = image.load(filename);
        if (valid){
            ui->centralWidget->activateWindow();
            ui->btm_restart->show();
            ui->lbl_mask->hide();
            ui->lbl_image->show();
        }
    }
}


void MainWindow::on_btm_restart_clicked()
{
    ui->btm_image->show();
    ui->label_1->show();
    hideVisual();
    ui->btm_restart->hide();
}


void MainWindow::on_btm_ShowLV_clicked()
{
    //Show Lightvectors
}


void MainWindow::hideVisual(){
    ui->btm_ShowLV->hide();
    ui->btm_restart->hide();
    ui->lbl_mask->hide();
    ui->lbl_image->hide();
}

void MainWindow::showVisual(){
    ui->btm_ShowLV->show();
}


void MainWindow::on_btm_Run_clicked(){

}

void MainWindow::on_toggle_btm_showMask_clicked(){

}










