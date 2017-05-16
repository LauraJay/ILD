#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>

QImage image;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->hide();
    ui->btm_StartSeg->hide();
    ui->btm_EndSeg->hide();
    ui->label_2->hide();
    ui->btm_ShowSeg->hide();
    ui->btm_ShowLV->hide();
    ui->btm_ShowAreas->hide();
    ui->btm_restart->hide();

    // read an image
    // cv::Mat image = cv::imread("C://Users//Laura//Documents//GUI//bild.jpg" , 1 );
    //cv::Mat image = cv::imread("F://FH//BV_projekt_ss17//Bilder//1.jpg" , 1 );
    // create image window named "My Image"
    // cv::namedWindow("My Image");
    // show the image on window
    // cv::imshow("My Image", image);
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
            ui->label->show();
            ui->btm_StartSeg->show();
            ui->btm_image->hide();
            ui->btm_restart->show();

        }
    }
}

void MainWindow::on_btm_StartSeg_clicked()
{
   // paintEvent();

    //Malen können und wenn genug Punkte vorhanden, kann EndSeg auf show
    bool Points = true;
    if(Points){
    ui->btm_EndSeg->show();
    }

}

void MainWindow::on_btm_EndSeg_clicked()
{
    //Malmodus beenden
    ui->label->hide();
    ui->btm_StartSeg->hide();
    ui->btm_EndSeg->hide();
    ui->label_2->show();
    ui->btm_ShowSeg->show();
    ui->btm_ShowLV->show();
    ui->btm_ShowAreas->show();
}

void MainWindow::on_btm_restart_clicked()
{
    ui->btm_image->show();
    ui->label->hide();
    ui->btm_StartSeg->hide();
    ui->btm_EndSeg->hide();
    ui->label_2->hide();
    ui->btm_ShowSeg->hide();
    ui->btm_ShowLV->hide();
    ui->btm_ShowAreas->hide();
    ui->btm_restart->hide();
    ui->lbl_image->clear();
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

//void MainWindow::paintEvent(QPaintEvent * event) {
  //this will draw your widget, and therefore the label too
  //QWidget::paintEvent(event);

  //then draw your point
  //Qpainter painter(this);
  //painter.drawPoint(x, y);
//}
