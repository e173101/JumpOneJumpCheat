#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QState>
#include <opencv2/core.hpp>
#include <QInputDialog>


#include "PComm.h"

#define liveWindowName "live"
#define screenWindowName "IPhone"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    joj(0.45),
    serialPort(15)
{
    ui->setupUi(this);
    cameraIsOk = false;
    //20 fps
    timer.setInterval(50);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timerDo()));
    /* Port Control, enable the port */
    int ret = sio_open (serialPort);
    if (ret == SIO_OK) {
        sio_ioctl (serialPort, B115200, P_NONE | BIT_8 | STOP_1 );
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    sio_close(1);
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
            length = abs(guyLoc.x-benchLoc.x);
            touchTimeMs = length*ratioLengthToTime;
            ui->label_interval->setText(QString::number(length));
            ui->label_touchms->setText(QString::number(touchTimeMs));

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
            timer.start();
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
        timer.stop();
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

void MainWindow::on_pushButton_pushScrean_clicked()
{
    char t[10];
    t[9]=0;
    touchTimeMs = ui->spinBox_touchms->value();
    sprintf(t,"%05dms",touchTimeMs);
    sio_write(serialPort,t,7);
}

void MainWindow::on_spinBox_touchms_editingFinished()
{
    qDebug("You Edit the touchTime");
}

void MainWindow::on_pushButton_push_clicked()
{
    char t[10];
    t[9]=0;
    sprintf(t,"%05dms",touchTimeMs);
    sio_write(serialPort,t,7);
}

void MainWindow::on_doubleSpinBox_ratio_editingFinished()
{
    ratioLengthToTime = ui->doubleSpinBox_ratio->value();
}
