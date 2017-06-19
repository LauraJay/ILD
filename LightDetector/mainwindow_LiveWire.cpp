//#include "mainwindow.h"
//#include "QtPanel.h"
//#include "QtGui"
//#include "ui_mainwindow.h"
//#include <opencv2/opencv.hpp>

//using namespace cv;

//QImage image;
//QVector<Contour*> Contours; //Alle fertigen Konturen
//QVector<QPoint> contourPoints; //Punkte während der Erstellung einer Kontur


//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);
//    hideSeg();
//    hideVisual();
//    hideRadioButton();
//    ui->btm_restart->hide();
//    ui->btm_backToSeg->hide();
//}

//void MainWindow::EndSingleContour(){
//        ui->btm_EndSingleSeg->hide();
//        ui->lbl_PressE->hide();
//        ui->lbl_PressE_2->hide();
//        ui->btm_StartSeg->show();
//        ui->btm_EndSeg->show();
//        ui->btm_DeleteConture->hide();
//        ui->lbl_CurrentCon->show();

//        c = new Contour(contourPoints);
//        Contours.push_back(c); //Aktuelle Kontur mit allen Anderen Konturen zs speichern

//        drawRadioButton();

//        //TO DO 1: Radio Buttons anzeigen
//        //TO DO 2: Gemalte Kontur aus Fenster löschen
//}

//void MainWindow::drawRadioButton(){
//    switch (Contours.size()) {
//        case 1: ui->rad_Con_1->show(); break;
//        case 2:{
//        ui->rad_Con_1->show();
//        ui->rad_Con_2->show();
//        break;}
//        case 3:{
//        ui->rad_Con_1->show();
//        ui->rad_Con_2->show();
//        ui->rad_Con_3->show();
//        break;}
//        case 4:{
//        ui->rad_Con_1->show();
//        ui->rad_Con_2->show();
//        ui->rad_Con_3->show();
//        ui->rad_Con_4->show();
//        break;}
//        case 5:{
//        ui->rad_Con_1->show();
//        ui->rad_Con_2->show();
//        ui->rad_Con_3->show();
//        ui->rad_Con_4->show();
//        ui->rad_Con_5->show();
//        break;}
//        case 6:{
//        ui->rad_Con_1->show();
//        ui->rad_Con_2->show();
//        ui->rad_Con_3->show();
//        ui->rad_Con_4->show();
//        ui->rad_Con_5->show();
//        ui->rad_Con_6->show();
//        break;}
//    }
//}


//MainWindow::~MainWindow()
//{
//    delete ui;
//    delete qtp;
//    delete c;
//}

//void MainWindow::on_btm_image_clicked()
//{
//    QString filename = QFileDialog::getOpenFileName(this, tr("Choose the Image you want to analyse"), "Testbilder",
//            tr("Images (*.jpg)"));
//    if (QString::compare(filename, QString()) !=0){
//        bool valid = image.load(filename);
//        if (valid){
//            openQtPanel(image.copy());
//            ui->centralWidget->activateWindow();
//            showSeg();
//            ui->btm_EndSeg->hide();
//            ui->btm_image->hide();
//            ui->label_1->hide();
//            ui->btm_restart->show();
//            qtp->setMouseEvent(false); //It is not possible to detect Contours.
//        }
//    }
//}




//void MainWindow::openQtPanel(QImage img){
//    qtp = new Livewire::QtPanel();
//    qtp->show();


//    if(img.height() <= ui->centralWidget->height()){
//        qtp->setFixedHeight(img.height());
//    } //scale to the maximum height
//    else if(img.height() > ui->centralWidget->height()){
//        qtp->setFixedHeight(ui->centralWidget->height()+32);
//        qtp->setFixedWidth((img.width()+32)*ui->centralWidget->height()/img.height());
//        img = img.scaledToHeight(centralWidget()->height()+32);
//    }
//    qtp->SetImage(img);
//    qtp->move(260,80);
//}

//void MainWindow::on_btm_StartSeg_clicked()
//{
//    //Segmentation is switched on
//    qtp->setMouseTracking(true);
//    qtp->setMouseEvent(true);
//    qtp->setIsSegDone(false);

//    hideRadioButton();
//    ui->btm_StartSeg->hide();
//    ui->btm_EndSingleSeg->show();
//    ui->lbl_PressE->show();
//     ui->lbl_PressE_2->show();
//    ui->btm_DeleteConture->show();







//}

//void MainWindow::on_btm_EndSingleSeg_clicked()
//{

//    //TO DO: Länge der Kontur abfragen und entscheiden,
//    //ob diese schon beendet werden kann.
//    contourPoints = qtp->points;
//    if(!qtp->getIsSegDone()){

//        QMessageBox::information(
//            this,
//            tr("Warning"),
//            tr("Please make sure to end your Segmentation using the Key 'E' before trying to save it.") );
//        qtp->activateWindow();
//    }
////    else if(contourPoints.size()<2){
////        QMessageBox::information(
////            this,
////            tr("Warning"),
////            tr("Your Contour is not long enough. Please make sure, that it contains at öeast two points.") );
////        qtp->activateWindow();
//   // }
//    else if(qtp->getIsSegDone()){
//        EndSingleContour();
//    }

//}

//void MainWindow::on_btm_EndSeg_clicked()
//{
//     hideSeg();
//     showVisual();
//     ui->btm_backToSeg->show();
//}

//void MainWindow::on_btm_restart_clicked()
//{
//    ui->btm_image->show();
//    ui->label_1->show();
//    hideSeg();
//    hideVisual();
//    ui->btm_restart->hide();
//    ui->btm_backToSeg->hide();
//    QImage blank = QImage();
//    qtp->SetImage(blank);
//}

//void MainWindow::on_btm_backToSeg_clicked()
//{
//    hideVisual();
//    ui->btm_backToSeg->hide();
//    ui->label_2->show();
//   // ui->btm_ChooseSeg->show();
//    ui->btm_StartSeg->show();
//    ui->btm_EndSeg->show();
//    ui->lbl_PressE->show();
//    ui->lbl_PressE_2->show();
//}

////void MainWindow::on_btm_ChooseSeg_clicked()
////{
////    //TO DO: Rausfinden, wie man auf Konturen zugreift
////   //Geignet alle vorhanden Konturen des Vektors Contours daretellen
////    //Möglichkeit bieten einzelne Konturen auuszuwählen
////}

//void MainWindow::on_btm_DeleteConture_clicked()
//{
//    if(!qtp->getIsSegDone()){

//        QMessageBox::information(
//            this,
//            tr("Warning"),
//            tr("Please make sure to end your Segmentation using the Key 'E' before trying to delete it.") );
//        qtp->activateWindow();
//    }
//    else{
//    //Konturen aus Vektor löschen
//    //Wenn keine Konturen mehr vorhanden sind, nur noch den Button zum neue Kontur erstellen anzeigen
//        //Nächste Buttomn aufrufen
//    }
//}

//void MainWindow::on_btm_ShowSeg_clicked()
//{
//    //show Segmentation
//}

//void MainWindow::on_btm_ShowLV_clicked()
//{
//    //Show Lightvectors
//}

//void MainWindow::on_btm_ShowAreas_clicked()
//{
//    //ShowAreas
//}

//void MainWindow::hideSeg(){
//    ui->label_2->hide();
//    ui->btm_StartSeg->hide();
//    ui->btm_EndSeg->hide();
//    ui->lbl_PressE->hide();
//     ui->lbl_PressE_2->hide();
//    //ui->btm_ChooseSeg->hide();
//    ui->btm_DeleteConture->hide();
//    ui->btm_EndSingleSeg->hide();
//}

//void MainWindow::showSeg(){
//    ui->label_2->show();
//    ui->btm_StartSeg->show();
//}

//void MainWindow::hideVisual(){
//    ui->label_3->hide();
//    ui->btm_ShowSeg->hide();
//    ui->btm_ShowLV->hide();
//    ui->btm_ShowAreas->hide();
//}

//void MainWindow::showVisual(){
//    ui->label_3->show();
//    ui->btm_ShowSeg->show();
//    ui->btm_ShowLV->show();
//    ui->btm_ShowAreas->show();
//}

//void MainWindow::hideRadioButton(){
//    ui->rad_Con_1->hide();
//    ui->rad_Con_2->hide();
//    ui->rad_Con_3->hide();
//    ui->rad_Con_4->hide();
//    ui->rad_Con_5->hide();
//    ui->rad_Con_6->hide();
//    ui->lbl_CurrentCon->hide();
//}










