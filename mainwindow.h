#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <QtGui>

#include "../PhoneHackCV/findscreen.h"
#include "../PhoneHackCV/jumponejump.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

Q_SIGNALS:
    void liveSignal();

private slots:

    void timerDo();

    void on_pushButton_openCamera_clicked();

    void on_pushButton_release_clicked();

    void on_pushButton_snapShot_clicked();

    void on_pushButton_pushScrean_clicked();

    void on_spinBox_touchms_editingFinished();

    void on_pushButton_push_clicked();

    void on_doubleSpinBox_ratio_editingFinished();

private:
    Ui::MainWindow *ui;
    //recored
    QTimer timer;
    int timerID;
    //camera
    VideoCapture camera;
    int cameraNum;
    bool cameraIsOk;
    //
    Mat matFrame;
    Mat screen;

    FindScreen fs;
    JumpOneJump joj;
    Point guyLoc,benchLoc;

    //searil
    int serialPort;
    //
    int length;
    double ratioLengthToTime;
    int touchTimeMs;
};

#endif // MAINWINDOW_H
