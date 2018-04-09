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
    //
    QStateMachine machine;
    QState *waitSourceState;
    QState *liveState;
    QState *recordingState;

    FindScreen fs;
    JumpOneJump joj;
    Point guyLoc,benchLoc;
    //
};

#endif // MAINWINDOW_H
