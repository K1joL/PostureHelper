#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Timer connet
    sessionTimer = new MyTimer(this);
    connect(sessionTimer->getTimer(), SIGNAL(timeout()),this, SLOT(timerSlot()));

    styleHelperFunction();
    //Stream define
    ui->videocam_feed->set_url("http://localhost/video_feed");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRun_model_triggered()
{
//    QString cmd("./model/MP.exe");
    QString cmd("D:/dev/python/python.exe D:/dev/QtProjects/App/model/MP1.py");
    QProcess::startDetached(cmd);
    ui->videocam_feed->start();
}

void MainWindow::on_actionStart_session_triggered()
{
    sessionTimer->startTimer(1000);
}

void MainWindow::timerSlot()
{
    sessionTimer->increaseTime();
    ui->StatsBlock_VgenTimer->setText(sessionTimer->getTime());
}

void MainWindow::on_updateCamBtn_clicked()
{
    updateCamBtnAnim->start();
    ui->videocam_feed->start();
}

void MainWindow::on_actionStop_Camera_triggered()
{
    ui->videocam_feed->stop();
}

void MainWindow::styleHelperFunction()
{
    //Animation connect
    SidePanelAnimHelper *SPAnimHelper = new SidePanelAnimHelper(this, ui->sidePanel, ui->sideBtn);
    connect(ui->sideBtn, &QPushButton::clicked, SPAnimHelper, &SidePanelAnimHelper::hideShowPanel);
    //Animation updateCamBtn
    static QIcon updCamBtnIcon(":/images/iconupdbtn.png");
    ui->updateCamBtn->setIconSize(QSize(48,48));
    ui->updateCamBtn->setIcon(updCamBtnIcon);
    updateCamBtnAnim = new QMovie(":/images/gifupdbtn.gif");
    connect(updateCamBtnAnim, &QMovie::frameChanged, [=]{
      ui->updateCamBtn->setIcon(updateCamBtnAnim->currentPixmap());
    });
    connect(ui->videocam_feed, &MJPEGStreamer::streaming,[=]{
        updateCamBtnAnim->stop();
        ui->updateCamBtn->setIcon(QIcon(updCamBtnIcon));
        ui->updateCamBtn->hide();
    });
    connect(ui->videocam_feed, &MJPEGStreamer::disconnected, [=]{
        ui->updateCamBtn->show();
    });
    //Animation fullscreen camera
    FSAnimHelper = new FullscreenCamAnimHelper(this, ui->videocam_feed, ui->updateCamBtn, ui->fullscrnBtn);
    connect(ui->fullscrnBtn, &QPushButton::clicked, FSAnimHelper, &FullscreenCamAnimHelper::onOffFullscreen);
}
