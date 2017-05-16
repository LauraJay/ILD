#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

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

    void on_btm_StartSeg_clicked();

    void on_btm_EndSeg_clicked();

    void on_btm_restart_clicked();

    void on_btm_ShowSeg_clicked();

    void on_btm_ShowLV_clicked();

    void on_btm_ShowAreas_clicked();

    void on_btm_EndSingleSeg_clicked();

    void on_btm_backToSeg_clicked();

private:
    Ui::MainWindow *ui;
    void showSeg();
    void hideSeg();
    void showVisual();
    void hideVisual();
};

#endif // MAINWINDOW_H
