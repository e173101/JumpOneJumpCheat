#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QState>
#include <opencv2/core.hpp>


#define liveWindowName "live"
#define screenWindowName "IPhone"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    joj(0.45)
{
    ui->setupUi(this);

    //
    cameraIsOk = false;
    //20 fps
    timer.setInterval(50);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timerDo()));

    // State Machine do
    //   waitResource
    //   live
    //   recording
    //

    //waitsouce
    waitSourceState = new QState();
    waitSourceState->assignProperty(ui->pushButton_openCamera, "enabled", true);
    waitSourceState->assignProperty(ui->pushButton_release, "enabled",false);
    waitSourceState->assignProperty(ui->pushButton_snapShot, "enabled",false);
    waitSourceState->assignProperty(ui->comboBox_live, "enabled",false);

    //live
    liveState = new QState();
    //live->killTimer(timerID);
    liveState->assignProperty(ui->pushButton_openCamera, "enabled", false);
    liveState->assignProperty(ui->pushButton_release, "enabled", true);
    liveState->assignProperty(ui->pushButton_snapShot, "enabled",true);
    liveState->assignProperty(ui->comboBox_live, "enabled",true);
    connect(liveState,SIGNAL(entered()),&timer,SLOT(start()));
    connect(liveState,SIGNAL(exited()),&timer,SLOT(stop()));

    //State transition
    waitSourceState->addTransition(this,SIGNAL(liveSignal()),liveState);
    liveState->addTransition(ui->pushButton_release, SIGNAL(clicked()),waitSourceState);
    //recording->addTransition(ui->pushButton_Record, SIGNAL(clicked()),live);

    //Init state machin
    machine.addState(waitSourceState);
   // machine.addState(recordingState);
    machine.addState(liveState);
    machine.setInitialState(waitSourceState);
    machine.start();

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timerDo(void)
{
    if(cameraIsOk)
    {
        camera >> matFrame;
        if(ui->comboBox_live->currentText()=="live")imshow(liveWindowName,matFrame);
        if(ui->comboBox_live->currentText()=="search screen")
        {
            fs.find(matFrame);
            imshow(liveWindowName,matFrame);
        }
        if(ui->comboBox_live->currentText()=="fix screen")
        {
            screen = fs.getScreen(matFrame);
            imshow(liveWindowName,matFrame);
            imshow(screenWindowName,screen);
        }
        if(ui->comboBox_live->currentText()=="cheat")
        {
            screen = fs.getScreen(matFrame);
            imshow(liveWindowName,matFrame);
            imshow(screenWindowName,screen);

            guyLoc = joj.matchGuy(screen);
            benchLoc = joj.matchBench(guyLoc);
            imshow("cheat",joj.matCooked);

        }

    }
}

void MainWindow::on_pushButton_openCamera_clicked()
{
    //setup the camera;
    cameraNum = QInputDialog::getInt(this,\
                                     tr("Select a camera, first is '0'"),\
                                     tr("camere number:"),\
                                     0,0,300,1,&cameraIsOk);
    if(cameraIsOk)
    {
        //test the camera
        cameraIsOk = camera.open(cameraNum);
        if(cameraIsOk)
        {
            qDebug("good");
            emit liveSignal();
        }
    }
    else
        ui->statusBar->showMessage("No a good camera :(",1000);
}

void MainWindow::on_pushButton_release_clicked()
{
    if(cameraIsOk)
    {
        camera.release();
        cameraIsOk=false;
    }
}

void MainWindow::on_pushButton_snapShot_clicked()
{
    QString imName;
    imName = QDate::currentDate().toString("yyyy_MM_dd")+QTime::currentTime().toString("_HH_mm_ss")+".jpg";
    imName.insert(0,"C:/Users/Public/Pictures/");
    //matFrame
    if(ui->comboBox_live->currentText()=="live")
        imwrite(imName.toStdString(), matFrame);

    if(ui->comboBox_live->currentText()=="search screen")
        imwrite(imName.toStdString(), matFrame);

    if(ui->comboBox_live->currentText()=="fix screen")
    {
        imwrite(imName.toStdString(), screen);
    }
}
