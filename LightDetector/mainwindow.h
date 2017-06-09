#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <contour.h>
#include <QtPanel.h>

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btm_image_clicked();

    void on_btm_restart_clicked();

    void on_btm_ShowLV_clicked();

    void on_btm_Run_clicked();

    void on_btm_selection_clicked();

    void on_btm_saveSelection_clicked();

    void on_btm_deleteSelection_clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    void showVisual();
    void hideVisual();
    cv::Mat QImage2Mat(QImage const& src);
    QImage Mat2QImage(cv::Mat const& src);
    void cropContour(QRect rect);
    void savePartOfContour();
    void paintRect();
    void paintStartPoint();
    void markNrOfContour();
    void deleteDrawnSelection();

};

#endif // MAINWINDOW_H
