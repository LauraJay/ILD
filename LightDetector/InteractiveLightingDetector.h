#ifndef INTERACTIVELIGHTINGDETECTOR_H
#define INTERACTIVELIGHTINGDETECTOR_H

#include <contour.h>

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class InteractiveLightingDetector : public QMainWindow
{
    Q_OBJECT

public:
    explicit InteractiveLightingDetector(QWidget *parent = 0);
    ~InteractiveLightingDetector();

private slots:
    void on_btm_image_clicked();

    void on_btm_StartSeg_clicked();

    void on_btm_EndSeg_clicked();

    void on_btm_restart_clicked();

    void on_btm_ShowSeg_clicked();

    void on_btm_ShowLV_clicked();

    void on_btm_ShowAreas_clicked();

    void on_btm_EndSingleSeg_clicked();

    void on_btm_backToSeg_clicked();

    void on_btm_ChooseSeg_clicked();

    void on_btm_DeleteConture_clicked();

private:
    Ui::MainWindow *ui;
    void showSeg();
    void hideSeg();
    void showVisual();
    void hideVisual();
};

#endif // MAINWINDOW_H
