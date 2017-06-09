#include "mainwindow.h"
#include "QtPanel.h"
#include "QtGui"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

QImage image;
QImage maskImage;
Mat imageInCV, imageCanny;
vector<vector<Point> > MainContour; //Muss wegen cv::findContour() von diesem Typ sein
vector<Point> SubContour; //Momentan kann zum Testen nur eine Subkontur erstellt werden
vector<Vec4i> hierarchy;
QRect CroppedRect;
QPen redPen, redPenThick, whitePen;
int translationX = 162; //Nur so passen die ausgeschnittenen Vierecke (muss ich noch verstehen)
int translationY = 2;


bool isSelect;
bool isDrawing;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hideVisual();
    ui->btm_Run->hide();
    ui->rad_Con1->hide();
    ui->btm_selection->hide();
    ui->btm_saveSelection->hide();
    ui->btm_deleteSelection->hide();
    ui->lbl_ListContours->hide();
    isSelect=false;
    redPen.setWidth(1);
    redPen.setColor(QColor(255,0,0));
    redPenThick.setWidth(14);
    redPenThick.setColor(QColor(255,0,0));
    whitePen.setWidth(3);
    whitePen.setColor(QColor(255,255,255));
   }



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btm_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose the Image you want to analyse"), "Testbilder/mitMasken",
            tr("Images (*.jpg)"));

    char* filenameCV = "C:\\Users\\Laura\\Desktop\\Projekte\\ILD\\build-LightDetector-Desktop_Qt_5_8_0_MinGW_32bit-Release\\Testbilder\\mitMasken\\TestImg_17_mask_glatt.jpg";
    imageInCV = imread(filenameCV, CV_8UC3); //momentan wird für openCV immer die gleiche Datei geladen


    if (QString::compare(filename, QString()) !=0){
        bool valid = image.load(filename);
        if (valid){
            image=image.scaledToWidth(ui->lbl_image->width(), Qt::SmoothTransformation);
           // ui->lbl_image->setPixmap(px);
            ui->lbl_image->setPixmap(QPixmap::fromImage(image));
            ui->centralWidget->activateWindow();
            ui->btm_restart->show();
            ui->lbl_mask->hide();
            ui->lbl_image->show();
            ui->btm_Run->show();
            ui->rad_Con1->hide();
            ui->btm_selection->hide();
            ui->btm_image->hide();
            printf("\n Bildgroesse width: %i und height: % i" , image.width(), image.height());
            printf("\n Labelgroesse width: %i und height: % i" , ui->lbl_image->width(), ui->lbl_image->height());
        }
    }
}


void MainWindow::on_btm_restart_clicked()
{
    ui->btm_image->show();
    ui->label_1->show();
    hideVisual();
    ui->btm_restart->hide();
    ui->btm_Run->hide();
    ui->rad_Con1->hide();
    ui->btm_selection->hide();
    ui->btm_deleteSelection->hide();
    ui->btm_saveSelection->hide();
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

      //Umwandeln des Bildes (Mache ich momentan in der btm_image_clicked() )
     // imageInCV = QImage2Mat(image);

      // Dilation to make sure that the Object is bigger than its mask
      int dilation_size = 1;
      Mat imageAfterMorph;
      Mat element = getStructuringElement(MORPH_RECT, Size(2*dilation_size+ 1 , 2*dilation_size+ 1), Point(dilation_size, dilation_size));
      dilate(imageInCV, imageAfterMorph, element );

      // Canny to detect edges
      Canny(imageAfterMorph, imageCanny, 0, 300 , 5);

      // Find associated Contours and draw them (in our case it) into the Mat imageWithContours
      findContours(imageCanny, MainContour, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
      printf("\n Anzahl Konturen: %i " , MainContour.size());
      Mat imageWithContours = Mat::zeros(imageInCV.size(), CV_8UC3);
      for (int i=0; i<MainContour.size(); i++){
         //drawContours( imageWithContours, MainContour, 1, Scalar (0, 255,0), 2, 8, hierarchy, 0, Point() );
         // drawContours( imageWithContours, MainContour, 0, Scalar (255, 0,0), 2, 8, hierarchy, 0, Point() );
         drawContours( imageWithContours, MainContour, i, Scalar (255, 255,255), 2, 8, hierarchy, 0, Point() );

      }

      //Display Contour as QImage
      image= Mat2QImage(imageWithContours);
      ui->lbl_image->setPixmap(QPixmap::fromImage(image));
      //imshow("Ausgangsbild", imageInCV);
      //imshow("Verkleinerte Kontur", imageAfterMorph);
      //imshow("Canny ", imageCanny);
      ui->btm_selection->show();
      ui->btm_Run->hide();
}

Mat MainWindow::QImage2Mat(QImage const& src){
    Mat tmp(src.height(), src.width(), CV_8UC3, (uchar*) src.bits(), src.bytesPerLine());
    Mat result;
    cvtColor(tmp, result, CV_BGR2RGB);
    //threshold(result, result, 120, THRESH_BINARY, THRESH_TOZERO);
    return result;
}

QImage MainWindow::Mat2QImage(Mat const& src){
    Mat tmp;
    cvtColor(src, tmp, CV_BGR2RGB);
    QImage resultLarge((const uchar*) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
    resultLarge.bits();
    //Bild so verkleinern, dass es passend in GUI angezeigt wird
    QImage result = resultLarge.scaled(image.width(), image.height(),Qt::KeepAspectRatio);
    printf("\n Bildgroesse Result width: %i und height: % i" , result.width(), result.height());
    printf("\n Bildgroesse Vorher width: %i und height: % i" , resultLarge.width(), resultLarge.height());
    return result;
}

void MainWindow::cropContour(QRect rect){
    QImage croppedImage = image.copy(rect);
    croppedImage.save("cropped_Image");
    ui->lbl_image->setPixmap(QPixmap::fromImage(croppedImage));
}

void MainWindow::savePartOfContour(){
    int xBoxHigh = CroppedRect.topRight().x();
    int xBoxLow = CroppedRect.topLeft().x();
    int yBoxHigh = CroppedRect.bottomRight().y();
    int yBoxLow = CroppedRect.topLeft().y();

    printf("\n Laenge der Subkontur zu Beginn: %i (Soll 0 sein!)" , SubContour.size());
    printf("\n Laenge der Kontur 1: %i" , MainContour.at(0).size());
    printf("\n Laenge der Kontur 2: %i" , MainContour.at(1).size());
    for (int i=0; i<MainContour.at(0).size(); i++){

        int xCon= MainContour.at(0).at(i).x - translationX;
        int yCon= MainContour.at(0).at(i).y + translationY;

        //Es werden noch zu wenig Konturenpunkte als innerhalb des Vierecks erkannt
        if(xBoxLow <= xCon && xCon <= xBoxHigh && yBoxLow <= yCon && yCon <= yBoxHigh){
            printf("\n Konturenpunkt liegt innerhalb der Auswahl");
            SubContour.push_back(Point(xCon,yCon));
            ui->rad_Con1->setChecked(true);
        }
    }
    printf("\n Laenge der Subkontur nach Selektierung: %i (Soll Anzahl aller Konturenpunkte innerhalb der Auswahl zählen)" , SubContour.size());
    if(SubContour.size()>=3){
        ui->lbl_ListContours->show();
        ui->rad_Con1->show();
    }
    else if(SubContour.size()<3){
        deleteDrawnSelection();
        QMessageBox::information(
                    this,
                    tr("Warning"),
                    tr("Selection was not saved, because its belonging Contour was too short for further calculations.") );

    }
}



void MainWindow::on_btm_selection_clicked()
{
    isSelect = true;
    ui->btm_saveSelection->show();
    ui->btm_deleteSelection->show();
    ui->btm_selection->hide();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
   if(isSelect){
      isDrawing = true;
    //isSelect = false;
    //Set Both Points to the same Starvalue
    CroppedRect.setTopLeft(QPoint(event->pos().rx()-translationX, event->pos().ry()+translationY));
    paintStartPoint();
    markNrOfContour();
    printf("\n Cropped Rect top left x=%i und y=%i", CroppedRect.topLeft().rx(), CroppedRect.topLeft().ry());
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){


       if(event->type() == QEvent::MouseMove && isDrawing){
          CroppedRect.setBottomRight(QPoint(event->pos().rx()-translationX, event->pos().ry()+translationY));
          //printf("\n Cropped Rect bottom right x=%i und y=%i", CroppedRect.bottomRight().rx(), CroppedRect.bottomRight().ry());
       }
}

   void MainWindow::mouseReleaseEvent(QMouseEvent *event){
        paintRect();
        if(isDrawing){
       //When mouse is released update for the one last time
       isSelect = false;
       isDrawing = false;
        }
   }



   void MainWindow::paintRect(){
       QPainter painter(&image);
      painter.setPen(redPen);
       if(isSelect && isDrawing){
           painter.drawRect(CroppedRect);
       }
       ui->lbl_image->setPixmap(QPixmap::fromImage(image));
       painter.end();
       paintStartPoint();
       markNrOfContour();
   }

   void MainWindow::paintStartPoint(){
       QPainter painter(&image);
      painter.setPen(redPenThick);
       if(isSelect && isDrawing){
           painter.drawPoint(CroppedRect.topLeft());
       }
       ui->lbl_image->setPixmap(QPixmap::fromImage(image));
       painter.end();
   }
//Momentan wird nur eine 1 gesetzt, weil nur eine Kontur möglich (ausbaufähig)
void MainWindow::markNrOfContour(){
       QPainter painter(&image);
      painter.setPen(whitePen);
       if(isSelect && isDrawing){
           painter.drawText(QPoint(CroppedRect.topLeft().rx()-2,CroppedRect.topLeft().ry()+3 ), "1");
       }
       ui->lbl_image->setPixmap(QPixmap::fromImage(image));
       painter.end();
 }




void MainWindow::on_btm_saveSelection_clicked(){
  ui->btm_saveSelection->hide();
  ui->btm_deleteSelection->hide();
  ui->btm_selection->show();
  //cropContour(CroppedRect);
  savePartOfContour();
}

void MainWindow::deleteDrawnSelection(){
    // TO DO
//    CroppedRect.setTopLeft(QPoint(0,0));
//    CroppedRect.setBottomRight(QPoint(0,0));
//    paintRect();
    //Delete from vector if saved there
}

void MainWindow::on_btm_deleteSelection_clicked()
{
   deleteDrawnSelection();
   ui->btm_selection->show();
   ui->btm_deleteSelection->hide();
   ui->btm_saveSelection->hide();
}
