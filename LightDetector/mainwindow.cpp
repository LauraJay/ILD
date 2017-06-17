#include "mainwindow.h"
#include "QtGui"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>


using namespace cv;
using namespace std;

QImage imageQT;
Mat maskImage, imageCV, imageCVwithContour;
vector<vector<Point> > MainContour; //Muss wegen cv::findContour() von diesem Typ sein
vector<Point> SubContour; //Momentan kann zum Testen nur eine Subkontur erstellt werden
vector<Point> SampledSubContour;
vector<Point> normals;
vector<Vec4i> hierarchy;
QString filename;
QRect CroppedRect;
QPen redPen, redPenThick, whitePen, bluePen;
int posImageLableX = 0;
int posImageLableY = 0;
int distanceOfNormals=10;


bool isSelect;
bool isDrawing;
bool isCon1Active;


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
    ui->btm_intensity->hide();
    isSelect=false;
    isCon1Active = false;
    redPen.setWidth(1);
    redPen.setColor(QColor(255,0,0));
    redPenThick.setWidth(14);
    redPenThick.setColor(QColor(255,0,0));
    whitePen.setWidth(3);
    whitePen.setColor(QColor(255,255,255));
    bluePen.setWidth(1);
    bluePen.setColor(QColor(0,0,255));
    }



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btm_image_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Choose the Image you want to analyse"), "Testbilder/mitMasken",
            tr("Images (*.jpg)"));
     std::string str = filename.toStdString();
     imageCV = imread(str.c_str(), CV_LOAD_IMAGE_COLOR);
      std::string filenameCV = str.substr(0,str.size()-4);
//     printf("\n", filenameCV.c_str());
     filenameCV+="_mask.jpg";
//     printf(filenameCV.c_str());

   Mat maskImageLoaded = imread(filenameCV.c_str(), CV_8UC3);

    if (QString::compare(filename, QString()) !=0){
        bool valid = imageQT.load(filename);
        if (valid){
            imageQT=imageQT.scaled(ui->lbl_image->width(),ui->lbl_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
            ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
            ui->centralWidget->activateWindow();
            ui->btm_restart->show();
            ui->lbl_mask->hide();
            ui->lbl_image->show();
            ui->btm_Run->show();
            ui->rad_Con1->hide();
            ui->btm_selection->hide();
            ui->btm_image->hide();
            cv::resize(maskImageLoaded, maskImage, Size(imageQT.width(), imageQT.height()));
            cv::resize(imageCV, imageCV, Size(imageQT.width(), imageQT.height()));

           // printf("\n Bildgroesse width: %i und height: % i" , imageQT.width(), imageQT.height());
           // printf("\n Labelgroesse width: %i und height: % i" , ui->lbl_image->width(), ui->lbl_image->height());
           // printf("\n Bildgroesse CV width: %i und height: % i" , maskImage.size().width, maskImage.size().height);
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
    ui->lbl_ListContours->hide();
    ui->btm_intensity->hide();
    ui->btm_ShowN->hide();
    SubContour.clear();
    MainContour.clear();
    hierarchy.clear();
}


void MainWindow::on_btm_ShowLV_clicked()
{

}

void MainWindow::on_btm_ShowN_clicked()
{
    setNormalVecs(distanceOfNormals);
    drawNormalVecs(distanceOfNormals);
    ui->btm_intensity->show();
    ui->btm_ShowN->hide();

}


void MainWindow::hideVisual(){
    ui->btm_ShowLV->hide();
    ui->btm_ShowN->hide();
    ui->btm_restart->hide();
    ui->lbl_mask->hide();
    ui->lbl_image->hide();
}

void MainWindow::showVisual(){
    ui->btm_ShowLV->show();
     ui->btm_ShowN->show();
}


void MainWindow::on_btm_Run_clicked(){

      //Umwandeln des Bildes (Mache ich momentan in der btm_image_clicked() )
     // imageInCV = QImage2Mat(image);

      // Dilation to make sure that the Object is bigger than its mask
      int dilation_size = 1;
      Mat imageAfterMorph;
      Mat element = getStructuringElement(MORPH_RECT, Size(2*dilation_size+ 1 , 2*dilation_size+ 1), Point(dilation_size, dilation_size));
      dilate(maskImage, imageAfterMorph, element );
       Mat imageCanny;
      // Canny to detect edges
      Canny(maskImage, imageCanny, 0, 1200 , 5);

      // Find associated Contours and draw them (in our case it) into the Mat imageWithContours
      findContours(imageCanny, MainContour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
      printf("\n Anzahl Konturen: %i " , MainContour.size());
     imageCVwithContour = imageCV.clone();
      for (int i=0; i<MainContour.size(); i++){
          drawContours( imageCVwithContour, MainContour, i, Scalar (0, 255,0), 2, 8, hierarchy, 0, Point() );

      }

      //Display Contour as QImage
      imageQT= Mat2QImage(imageCVwithContour);
      ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
      //imshow("Ausgangsbild", imageInCV);
      //imshow("Verkleinerte Kontur", imageAfterMorph);
     // imshow("Canny ", imageCanny);
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
    QImage result = resultLarge.scaled(imageQT.width(), imageQT.height(),Qt::KeepAspectRatio);
//    printf("\n Bildgroesse Result width: %i und height: % i" , result.width(), result.height());
//    printf("\n Bildgroesse Vorher width: %i und height: % i" , resultLarge.width(), resultLarge.height());
    return result;
}

void MainWindow::cropContour(QRect rect){
    QImage croppedImage = imageQT.copy(rect);
    croppedImage.save("cropped_Image");
    ui->lbl_image->setPixmap(QPixmap::fromImage(croppedImage));
}

void MainWindow::savePartOfContour(){
    int xBoxHigh = CroppedRect.topRight().x();
    int xBoxLow = CroppedRect.topLeft().x();
    int yBoxHigh = CroppedRect.bottomRight().y();
    int yBoxLow = CroppedRect.topLeft().y();

    printf("\n Laenge der Kontur insgesamt: %i " , MainContour.at(0).size());
    printf("\n Laenge der Subkontur zu Beginn: %i (Soll 0 sein!)" , SubContour.size());
    Mat imageDebug = Mat::zeros(maskImage.size(), CV_8UC3);
    Rect r = Rect(Point(xBoxHigh,yBoxHigh),Point(xBoxLow,yBoxLow));
    rectangle(imageDebug,r,Scalar(0,0,255));


    for (int i=0; i<MainContour.at(0).size(); i++){
        int xCon= MainContour.at(0).at(i).x;
        int yCon= MainContour.at(0).at(i).y;

        if (CroppedRect.contains(xCon, yCon)){
           // printf("\n Konturenpunkt liegt innerhalb der Auswahl");
            SubContour.push_back(Point(xCon,yCon));
            ui->rad_Con1->setChecked(true);
            circle(imageDebug,Point(xCon,yCon),1,Scalar(0,255,0));

        }
        else{

        circle(imageDebug,Point(xCon,yCon),1,Scalar(255,0,0));

        }
    }
    printf("\n Laenge der Subkontur nach Selektierung: %i (Soll Anzahl aller Konturenpunkte innerhalb der Auswahl zaehlen)" , SubContour.size());
    if(SubContour.size()>=3){
        ui->lbl_ListContours->show();
        ui->rad_Con1->show();
        sortSubContour();
        paintSubContour(0);
       imshow("Debug", imageDebug);

       computePixelCoordsAlongContour();
       printf("\n Laenge der SampledSubKontur insgesamt: %i " , SampledSubContour.size());

      }
    else if(SubContour.size()<3){
        deleteDrawnSelection();
        QMessageBox::information(
                    this,
                    tr("Warning"),
                    tr("Selection was not saved, because its belonging Contour was too short for further calculations.") );

    }
}

void  MainWindow::computePixelCoordsAlongContour(){
    for (int i = 0; i < SubContour.size()-1; ++i) {
        Mat tempLineImg = maskImage.clone();
        tempLineImg= Mat::zeros(tempLineImg.size(),tempLineImg.type());
        line(tempLineImg,SubContour[i],SubContour[i+1],Scalar(255,255,255));
        if (i==SubContour.size()-2) {
            imshow("lastLine",tempLineImg);
        }
        runLineIterator(tempLineImg, SubContour[i],SubContour[i+1]);
        //tempLineImg.release();
    }
    Mat debug = imageCV.clone();
    circle(debug,SampledSubContour[0],5,Scalar(255,0,0));
    circle(debug,SampledSubContour[SampledSubContour.size()-1],5,Scalar(255,0,0));

    foreach (Point c, SampledSubContour) {
    circle(debug,c,1,Scalar(0,0,255));
    }
    imshow("pixel Contour",debug);

}

void MainWindow::runLineIterator(Mat tempLineImg,Point pt1, Point pt2){
    // grabs pixels along the line (pt1, pt2)
    // from 8-bit 3-channel image to the buffer
    LineIterator it(tempLineImg, pt1, pt2, 8, true);
    for(int i = 0; i < it.count; i++, ++it){
       Point buffer = it.pos();
       SampledSubContour.push_back(buffer);
    }
   }

void MainWindow:: sortSubContour(){
    // apply it to the contours:
    vector<Point> SortedSubContour;
    Point mostLeftDown = Point(imageCV.size().width,0);
    Point mostRightDown = Point(0,0);

    int mostLeftPos = 0;
    int mostRightPos = 0;
    for (int i = 0; i < SubContour.size()-1; ++i) {
        Point p = SubContour[i];
        if(p.x<=mostLeftDown.x && p.y >= mostLeftDown.y){
            mostLeftDown = p;
            mostLeftPos=i;
        }
        if(p.x>=mostRightDown.x && p.y >= mostRightDown.y){
            mostRightDown = p;
            mostRightPos=i;
        }
    }

   for (int i = mostRightPos; i < SubContour.size(); ++i) {
        SortedSubContour.push_back(SubContour[i]);
    }
   for (int i = 0; i < mostLeftPos; ++i) {
    SortedSubContour.push_back(SubContour[i]);
   }
   SubContour = SortedSubContour;
  //  std::sort(SubContour.begin(), SubContour.end(), contour_sorter());
}

void MainWindow::on_btm_selection_clicked()
{
    isSelect = true;
//    ui->btm_saveSelection->show();
//    ui->btm_deleteSelection->show();
    ui->btm_selection->hide();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
   if(isSelect){
      isDrawing = true;
      posImageLableX = ui->lbl_image->pos().x()+ui->centralWidget->pos().x()+ui->toggle_btm_showMask->pos().x();
      posImageLableY = ui->lbl_image->pos().y()+ ui->centralWidget->pos().y()+ui->toggle_btm_showMask->pos().y();
    //isSelect = false;
    //Set Both Points to the same Starvalue
    CroppedRect.setTopLeft(QPoint(event->pos().x()-posImageLableX, event->pos().y()-posImageLableY));
    paintStartPoint();
    markNrOfContour();
    printf("\n Cropped Rect top left x=%i und y=%i", CroppedRect.topLeft().x(), CroppedRect.topLeft().y());
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){


       if(event->type() == QEvent::MouseMove && isDrawing){
           posImageLableX = ui->lbl_image->pos().x()+ui->centralWidget->pos().x()+ui->toggle_btm_showMask->pos().x();
           posImageLableY = ui->lbl_image->pos().y()+ ui->centralWidget->pos().y()+ui->toggle_btm_showMask->pos().y();
          CroppedRect.setBottomRight(QPoint(event->pos().x()-posImageLableX, event->pos().y()-posImageLableY));
          //printf("\n Cropped Rect bottom right x=%i und y=%i", CroppedRect.bottomRight().x(), CroppedRect.bottomRight().y());
           paintRect();
       }
}

   void MainWindow::mouseReleaseEvent(QMouseEvent *event){
       paintRect();
       if(isDrawing){
       //When mouse is released update for the one last time
       isSelect = false;
       isDrawing = false;
       ui->btm_saveSelection->show();
       ui->btm_deleteSelection->show();
        }
   }

   void MainWindow::paintSubContour(int n){
       Mat temp = imageCV.clone();
        vector<vector<Point> > SubContours;
        SubContours.push_back(SubContour);
        drawContours( temp, SubContours, n, Scalar (0, 255,0), 2, 8, hierarchy, 0, Point() );
       imageQT= Mat2QImage(temp);
       ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
        }


   void MainWindow::paintRect(){
       imageQT= Mat2QImage(imageCVwithContour);
       QPainter painter(&imageQT);
      painter.setPen(redPen);
       if(isSelect && isDrawing){
           painter.drawRect(CroppedRect);
       }
       ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
       painter.end();
       paintStartPoint();
       markNrOfContour();
   }

   void MainWindow::paintStartPoint(){
       QPainter painter(&imageQT);
      painter.setPen(redPenThick);
       if(isSelect && isDrawing){
           painter.drawPoint(CroppedRect.topLeft());
       }
       ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
       painter.end();
   }
//Momentan wird nur eine 1 gesetzt, weil nur eine Kontur möglich (ausbaufähig)
void MainWindow::markNrOfContour(){
       QPainter painter(&imageQT);
      painter.setPen(whitePen);
       if(isSelect && isDrawing){
           painter.drawText(QPoint(CroppedRect.topLeft().rx()-2,CroppedRect.topLeft().ry()+3 ), "1");
       }
       ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
       painter.end();
 }




void MainWindow::on_btm_saveSelection_clicked(){
  ui->btm_saveSelection->hide();
  ui->btm_deleteSelection->hide();
  ui->btm_ShowLV->show();
   ui->btm_ShowN->show();
  //cropContour(CroppedRect);
  savePartOfContour();
  isCon1Active = true;
}

void MainWindow::deleteDrawnSelection(){
    //optische Löschung der Kontur
    ui->lbl_image->clear();
    bool valid = imageQT.load(filename);
    if(valid){
     imageQT=imageQT.scaled(ui->lbl_image->width(),ui->lbl_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
    }

    //Löschen des Konturenvektors:
    //Löschen der Normalen und des Subkonturen falls wir Button auch verwenden wollen um nachträglich Konturen zu löschen :)
    printf("\n \n Anzahl Main-Normale vor Loeschung: %i", MainContour.size());
    printf("\n Anzahl Sub-Normalen vor Loeschung: %i", normals.size());
    printf("\n Anzahl Subkonturen vor Loeschung: %i", SubContour.size());
    MainContour.erase(MainContour.begin(), MainContour.end());
    SubContour.erase(SubContour.begin(), SubContour.end());
    normals.erase(normals.begin(),normals.end());
    printf("\n \n Anzahl Main-Normale nach Loeschung: %i", MainContour.size());
    printf("\n Anzahl Sub-Normalen nach Loeschung: %i", normals.size());
    printf("\n Anzahl Subkonturen nach Loeschung: %i", SubContour.size());

}

void MainWindow::on_btm_deleteSelection_clicked()
{
   deleteDrawnSelection();
   ui->btm_selection->show();
   ui->btm_deleteSelection->hide();
   ui->btm_saveSelection->hide();
}

void MainWindow::on_rad_Con1_toggled(bool checked)
{
    if(checked){
        paintSubContour(0);
        //printf("checked");
        isCon1Active = true;
    }
    else if(!checked){
        deleteDrawnSelection();
      // printf("not checked");
       isCon1Active= false;
    }
}

void MainWindow::setNormalVecs(int distance){
    for(int i = 0; i <SampledSubContour.size()-distance; i+=distance){
        Point startPos = SampledSubContour.at(i);
        Point endPos = SampledSubContour.at(i+distance);
        int dx = endPos.x - startPos.x;
        int dy = endPos.y - startPos.y;

        //Point normalOne = Point(-dy,dx); //Scheinen wir nicht zu brauchen, da diese Normale immer in dem Objekt liegt
        Point normalTwo = Point(dy,-dx);
        //TO DO: Länge der Normalen angleichen
        normals.push_back(normalTwo);
    }
    printf("\n Anzahl Normalen: %i" , normals.size());

}

void MainWindow::drawNormalVecs(int distance){
      QPainter normalPainter(&imageQT);
      normalPainter.setPen(redPen);
      int counter = 0;
      int i=0;
      int endX, endY;
      while(counter < normals.size()){
        endX = SampledSubContour.at(i+distance/2).x - normals.at(counter).x;
        endY = SampledSubContour.at(i+distance/2).y - normals.at(counter).y;
        normalPainter.drawLine(SampledSubContour.at(i+distance/2).x,SampledSubContour.at(i+distance/2).y,endX,endY);
        counter ++;
        i +=distance;
      }
//      normalPainter.setPen(bluePen);
//      normalPainter.drawLine(SubContour.at(i-1+distance/2).x,SubContour.at(i-1+distance/2).y,endX,endY);// Letzte Normale der Kontur
//      normalPainter.setPen(whitePen);
//      int a = SubContour.at(0).x - normals.at(0).x;
//      int b = SubContour.at(0).y - normals.at(0).y;
//      normalPainter.drawLine(SubContour.at(0).x, SubContour.at(0).y,a,b ); //Erste Normale der Kontur

     ui->lbl_image->setPixmap(QPixmap::fromImage(imageQT));
     normalPainter.end();
}

//R ist Einheitszahl= 1
//3 Unbekannte: L.x, L.y und A (int oder float?)
//Ist die Frage ob wir die Intensity überhaupt berechnen müssen, weil wir ja eigentlich nur L rausbekommen wollen.
void MainWindow::calculateIntensity(int R, vector<Point> N, vector<Point> L, int A){
    int NL;

    for (int i=0; i<L.size(); i++){
    NL = N.at(i).x * L.at(i).x;
    NL+= N.at(i).y * L.at(i).y;
    int I = R * NL +A;
    printf("\n Die Intensitaet an Normale %i liegt bei: %i ",i, I);
}
}

void MainWindow::on_btm_intensity_clicked()
{
    vector<Point> L;
    L.push_back(Point(2,3));
     L.push_back(Point(3,1));
      L.push_back(Point(2,4));
    calculateIntensity(1, normals, L, 4);
    ui->btm_intensity->hide();
}
