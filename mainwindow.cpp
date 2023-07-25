#include "mainwindow.h"
#include "./ui_mainwindow.h"

/*
 * TODO LIST:
 * Make rating side panel like additional widget
 * Make current state block like additional widget
 *
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sessionTimer = new MyTimer();
    usefulTimer = new MyTimer();
    askSwitchToExercise = new dialogWindowWidget(this);
    //ClientBLock define
    clientBlock = new ClientInfoWidget(ui->MainInfoFrame);
    //Stream define
    availExercWidget = new AvailableExerciseWidget(this);
    videoCamWidget = new VideoCamFeedWidget(QString("http://localhost/video_feed"), this);
    connect(videoCamWidget->getUpdateButton(),  &QPushButton::clicked,
            this, &MainWindow::updateCamBtnclicked);
    controlWindow = new ControlWindowWidget(usefulTimer, availExercWidget, this);
    exerciseWindow = new ExerciseWindowWidget(usefulTimer, availExercWidget, this);
    //connect two additional windows
    connect(controlWindow, &ControlWindowWidget::sig_ctrlWindowToMain,
            this, &MainWindow::show);
    connect(exerciseWindow, &ExerciseWindowWidget::sig_exerciseWindowToMain,
            this, &MainWindow::show);
    connect(controlWindow->getDialogOkBtn(), &QPushButton::clicked,
            this, &MainWindow::on_CtrlWindDialogOkBtnClicked);
    connect(exerciseWindow->getDialogStartCancelBtn(), &QPushButton::clicked,
            this, &MainWindow::on_ExerWindDialogStartCancelClicked);
    connect(exerciseWindow, &ExerciseWindowWidget::sig_startExerciseClicked,
            this, &MainWindow::on_startExerciseClicked);
    connect(exerciseWindow, &ExerciseWindowWidget::sig_startExerciseStand,
            this, &MainWindow::on_startExerciseStand);
    connect(exerciseWindow, &ExerciseWindowWidget::sig_exerciseCompleted,
            this, &MainWindow::on_completeExercise);
    connect(availExercWidget, &AvailableExerciseWidget::sig_exerciseButtonClicked,
            this, &MainWindow::on_exerciseButtonClicked);
    connect(askSwitchToExercise->getOkBtn(), &QPushButton::clicked,
            this, &MainWindow::on_exerciseModeBtn_clicked);
    //connect additional windows to each other
    controlWindow->setExerciseWindow(exerciseWindow);
    exerciseWindow->setControlWindow(controlWindow);
    connect(controlWindow, &ControlWindowWidget::sig_ctrlWindowToExercise,
            exerciseWindow, &ExerciseWindowWidget::showWindow);
    connect(exerciseWindow, &ExerciseWindowWidget::sig_exerciseWindowToCtrl,
            controlWindow, &ControlWindowWidget::showWindow);
    connect(exerciseWindow, &ExerciseWindowWidget::sig_cancelExerciseClicked,
            controlWindow, &ControlWindowWidget::on_exerciseCancel);

    //Connect widgets to windows
    controlWindow->setVideoCamFeedWidget(videoCamWidget);
    exerciseWindow->setVideoCamFeedWidget(videoCamWidget);
    controlWindow->setClientBlock(clientBlock);
    exerciseWindow->setClientBlock(clientBlock);
    //Timer connect
    connect(sessionTimer->getTimer(), SIGNAL(timeout()), this, SLOT(genTimerSlot()));
    connect(usefulTimer->getTimer(), SIGNAL(timeout()), this, SLOT(usefulTimerSlot()));
    connect(usefulTimer, &MyTimer::sig_increasePoints, this, &MainWindow::increasePointsForTime);
    messageSocket = new MessageSocket(QHostAddress::LocalHost, 4444, 12345);
    //add exercises in list
    availExercWidget->addExercise(new ExerciseObject(QString("Squats"),QString("Приседания"),15,20,10,                                  //Приседания
                                                     QString("Выполнять упражнение встав к камере боком."
                                                             "Встаньте ровно и поставьте ноги на ширине плеч.\n"
                                                             "Во время выполнения упражнения держите спину прямо.\n"
                                                             "После чего начните медленно опускать бедра,\n"
                                                             "пока они не станут параллельны полу.\n"
                                                             "В нижнем положении задержитесь на 1-2 секунды,\n"
                                                             "и вернитесь в начальное положение.\n"
                                                             "Вес тела должен быть на пятках.\n"
                                                             "Руки можно выставить вперед, для поддержания баланса.\n")));
    availExercWidget->addExercise(new ExerciseObject(QString("Sbends"),QString("Наклоны в стороны"),10,10,5,                            //Наклоны в стороны
                                                     QString("Выполнять упражнение встав к камере передом."
                                                             "Встаньте ровно и поставьте ноги на ширине плеч.\n"
                                                             "Выполните наклон в сторону,\n"
                                                             "выпрямитесь и выполните наклон в другую сторону.\n"
                                                             "Во время наклона держите туловище прямо,\n"
                                                             "не допуская наклонов вперед и назад.\n"
                                                             "Не сутультесь и не округляйте плечи.\n"
                                                             "Держите грудь расправленной. лопатки сведенными.")));
    availExercWidget->addExercise(new ExerciseObject(QString("Fbends"),QString("Наклоны вперед"),10,10,5,                               //Наклоны вперед
                                                     QString("Выполнять упражнение встав к камере боком."
                                                             "Встаньте ровно и поставьте ноги на ширине плеч.\n"
                                                             "Выполните наклон вперед\n"
                                                             "Выполнять упражнение нужно плавно, без рывков.\n"
                                                             "В процессе выполнения упражнения следить за тем,\n"
                                                             "чтобы корпус не тянулся за счет мышц спины,\n"
                                                             "это может привести к травме!\n")));
    //connect socket signals
    connect(messageSocket, &MessageSocket::sig_error_back_received,
            this, &MainWindow::on_errorBack);
    connect(messageSocket, &MessageSocket::sig_error_neck_received,
            this, &MainWindow::on_errorNeck);
    connect(messageSocket, &MessageSocket::sig_error_rLeg_received,
            this, &MainWindow::on_errorRLeg);
    connect(messageSocket, &MessageSocket::sig_error_lLeg_received,
            this, &MainWindow::on_errorLLeg);
    connect(messageSocket, &MessageSocket::sig_error_none_received,
            this, &MainWindow::on_errorNone);
    connect(messageSocket, &MessageSocket::sig_exercise_ready_received,
            this, &MainWindow::on_exerciseReady);
    connect(messageSocket, &MessageSocket::sig_exercise_notReady_received,
            this, &MainWindow::on_exerciseNotReady);
    connect(messageSocket, &MessageSocket::sig_exercise_did_received,
            this, &MainWindow::on_exerciseDid);
    //connect videocam signals
    connect(videoCamWidget, &VideoCamFeedWidget::sig_SkeletonHide,
            this, &MainWindow::on_skeletonHide);
    connect(videoCamWidget, &VideoCamFeedWidget::sig_SkeletonShow,
            this, &MainWindow::on_skeletonShow);
    styleHelperFunction();
    QString cmd("./model/MP1.exe");
//    QString cmd("D:/dev/python/python.exe D:/dev/QtProjects/App/model/MP1.py");
    QProcess::startDetached(cmd);
    videoCamWidget->startVideoStream();
    sessionTimer->startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    messageSocket->send(QString("Stop"));
    event->accept();
}

void MainWindow::genTimerSlot()
{
    sessionTimer->increaseTime();
    ui->StatsBlock_VgenTimer->setText(sessionTimer->getTime());
}

void MainWindow::usefulTimerSlot()
{
    if(isPostureCorrect)
    {
        usefulTimer->increaseTime();
        ui->StatsBlock_VusefulTimer->setText(usefulTimer->getTime());
    }
}

void MainWindow::increasePointsForTime()
{

    clientBlock->addPoints(2);
    saldoPoints += 2;
}

void MainWindow::decreasePoint()
{
    clientBlock->addPoints(-2);
    saldoPoints -= 2;
}

void MainWindow::updateCamBtnclicked()
{
    videoCamWidget->startVideoStream();
}

void MainWindow::styleHelperFunction()
{
    clientBlock->setGeometry(160,10,91,111);
    availExercWidget->setGeometry(350, 50,
                                  availExercWidget->width(), availExercWidget->height());
    videoCamWidget->setGeometry(ui->MainInfoFrame->x()+10,ui->MainInfoFrame->y()+10,
                                videoCamWidget->width(),videoCamWidget->height());
    this->setWindowIcon(QIcon(":/images/icon.png"));
    ui->sideBtn->hide();
    askSwitchToExercise->hide();
    askSwitchToExercise->setHeader("Вопрос");
    askSwitchToExercise->setText("Хотите перейти к выполнению упражения?");
}

void MainWindow::on_controlModeBtn_clicked()
{
    videoCamWidget->setParent(controlWindow);
    videoCamWidget->setWholeGeometry(360,60,611,481);
    videoCamWidget->hideFullscreenBtn();
    availExercWidget->setParent(controlWindow);
    availExercWidget->hide();
    clientBlock->setParent(controlWindow);
    controlWindow->showWindow();
}

void MainWindow::on_exerciseModeBtn_clicked()
{
    videoCamWidget->setParent(exerciseWindow);
    videoCamWidget->setWholeGeometry(360,60,611,481);
    videoCamWidget->hideFullscreenBtn();
    availExercWidget->setParent(exerciseWindow);
    availExercWidget->hide();
    clientBlock->setParent(exerciseWindow);
    exerciseWindow->showWindow();
    askSwitchToExercise->hide();
}

void MainWindow::on_exerciseReady()
{
    //TODO: Ready signal processing
}

void MainWindow::on_exerciseNotReady()
{
    exerciseWindow->showDialogNotReady();
}

void MainWindow::on_exerciseDid()
{
    exerciseWindow->decreaseRepCounter();
}

void MainWindow::on_errorNeck()
{
    ui->CurStBlock_Vneck->setStyleSheet("#CurStBlock_Vneck {color: red;}");
    controlWindow->setNeckRed();
    isPostureCorrect = false;
}

void MainWindow::on_errorBack()
{
    ui->CurStBlock_Vback->setStyleSheet("#CurStBlock_Vback {color: red;}");
    controlWindow->setBackRed();
    isPostureCorrect = false;
}

void MainWindow::on_errorLLeg()
{
    ui->CurStBlock_Vlleg->setStyleSheet("#CurStBlock_Vlleg {color: red;}");
    controlWindow->setLeftLegRed();
    isPostureCorrect = false;
}

void MainWindow::on_errorRLeg()
{
    ui->CurStBlock_Vrleg->setStyleSheet("#CurStBlock_Vrleg {color: red;}");
    controlWindow->setRightLegRed();
    isPostureCorrect = false;
}

void MainWindow::on_errorNone()
{
    ui->CurStBlock_Vrleg->setStyleSheet("#CurStBlock_Vrleg {color: white;}");
    ui->CurStBlock_Vneck->setStyleSheet("#CurStBlock_Vneck {color: white;}");
    ui->CurStBlock_Vback->setStyleSheet("#CurStBlock_Vback {color: white;}");
    ui->CurStBlock_Vlleg->setStyleSheet("#CurStBlock_Vlleg {color: white;}");
    controlWindow->setPartsWhite();
    isPostureCorrect = true;
}

void MainWindow::on_completeExercise()
{
    messageSocket->send(QString("Pause"));
    clientBlock->addPoints(exerciseWindow->getCurrentExercise()->getPoints());
}

void MainWindow::on_CtrlWindDialogOkBtnClicked()
{
    messageSocket->send(QString("Calibration"));
}

void MainWindow::on_startExerciseClicked()
{
    messageSocket->send(exerciseWindow->getCurrentExercise()->getCodeName());
}

void MainWindow::on_startExerciseStand()
{
    messageSocket->send(QString("Stand"));
}

void MainWindow::on_ExerWindDialogStartCancelClicked()
{
    messageSocket->send(QString("Pause"));
}

void MainWindow::on_skeletonHide()
{
    messageSocket->send(QString("Hide"));
}

void MainWindow::on_skeletonShow()
{
    messageSocket->send(QString("Show"));
}

void MainWindow::on_exerciseButtonClicked()
{
    if(availExercWidget->parentWidget() != this)
        return;
    askSwitchToExercise->show();
    askSwitchToExercise->raise();
}
