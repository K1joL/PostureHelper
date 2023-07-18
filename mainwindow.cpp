#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SessionTimer = new MyTimer(this);
    connect(SessionTimer->getTimer(), SIGNAL(timeout()),this, SLOT(timerSlot()));
    SidePanelAnimHelper *helper = new SidePanelAnimHelper(ui->sidePanel, ui->sideBtn, this);
    connect(ui->sideBtn, &QPushButton::clicked, helper, &SidePanelAnimHelper::hideShowPanel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRun_model_triggered()
{
    QString path("./resources/model/MP/MP.exe");
    QProcess::startDetached(path);
}

void MainWindow::on_actionStart_session_triggered()
{
    SessionTimer->startTimer(1000);
}

void MainWindow::timerSlot()
{
    SessionTimer->increaseTime();
    ui->StatsBlock_VgenTimer->setText(SessionTimer->getTime());
}

