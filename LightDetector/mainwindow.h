#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <contour.h>
#include <QtPanel.h>

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

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

    void on_btm_mask_clicked();

    void on_btm_restart_clicked();

    void on_btm_ShowLV_clicked();

    void on_btm_Run_clicked();

    void on_toggle_btm_showMask_clicked();



private:
    Ui::MainWindow *ui;
    void showVisual();
    void hideVisual();
};

#endif // MAINWINDOW_H
