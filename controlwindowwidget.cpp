#include "controlwindowwidget.h"
#include "ui_controlwindowwidget.h"

ControlWindowWidget::ControlWindowWidget(MyTimer *usefulTimer, AvailableExerciseWidget* availExercWidget, QWidget *parent) :
    usefulTimerC(usefulTimer),
    availExercWidget(availExercWidget),
    QWidget(parent),
    ui(new Ui::ControlWindowWidget)
{
    usefulTimerC->startTimer(1000);
    usefulTimerC->pause();
    ui->setupUi(this);
    calibrationTimer = new QTimer(this);
    calibrationDialogWidget = new dialogWindowWidget(this);
    askSwitchToExercise = new dialogWindowWidget(this);
    calibrationCounterLbl = new QLabel(calibrationDialogWidget->getFrameStyle());
    styleHelperFunction();
    connect(usefulTimer->getTimer(), SIGNAL(timeout()),
            this, SLOT(ctrlTimerSlot()));
    connect(calibrationDialogWidget->getOkBtn(), &QPushButton::clicked,
            this, &ControlWindowWidget::on_startOkBtnClicked);
    connect(calibrationDialogWidget->getCancelBtn(), &QPushButton::clicked,
            this, &ControlWindowWidget::on_startCancelBtnClicked);
    connect(calibrationTimer, &QTimer::timeout,
            this, &ControlWindowWidget::decreaseCalibrationCounter);
    connect(askSwitchToExercise->getOkBtn(), &QPushButton::clicked,
            this, &ControlWindowWidget::on_exerciseModeBtnC_clicked);
    connect(availExercWidget, &AvailableExerciseWidget::sig_exerciseButtonClicked,
            this, &ControlWindowWidget::on_exerciseButtonClicked);
}

ControlWindowWidget::~ControlWindowWidget()
{
    delete ui;
}

void ControlWindowWidget::setBackRed()
{
    ui->CurStBlock_Vback->setStyleSheet("#CurStBlock_Vback {color: red;}");
}

void ControlWindowWidget::setNeckRed()
{
    ui->CurStBlock_Vneck->setStyleSheet("#CurStBlock_Vneck {color: red;}");
}

void ControlWindowWidget::setLeftLegRed()
{
    ui->CurStBlock_Vlleg->setStyleSheet("#CurStBlock_Vlleg {color: red;}");
}

void ControlWindowWidget::setRightLegRed()
{
    ui->CurStBlock_Vrleg->setStyleSheet("#CurStBlock_Vrleg {color: red;}");
}

void ControlWindowWidget::setPartsWhite()
{
    ui->CurStBlock_Vrleg->setStyleSheet("#CurStBlock_Vrleg {color: white;}");
    ui->CurStBlock_Vneck->setStyleSheet("#CurStBlock_Vneck {color: white;}");
    ui->CurStBlock_Vback->setStyleSheet("#CurStBlock_Vback {color: white;}");
    ui->CurStBlock_Vlleg->setStyleSheet("#CurStBlock_Vlleg {color: white;}");
}

void ControlWindowWidget::showWindow()
{
    this->show();
    videoCamFeed->setParent(ui->videofeed_frameC);
    videoCamFeed->setWholeGeometry(10,10,611,481);
    videoCamFeed->show();
    clientBlock->setParent(ui->MainInfoFrameC);
    clientBlock->setGeometry(10,10,91,111);
    clientBlock->show();
}

void ControlWindowWidget::styleHelperFunction()
{
    this->hide();
    calibrationDialogWidget->hide();
    QFrame *styleFrame = calibrationDialogWidget->getFrameStyle();
    calibrationDialogWidget->setHeader("Калибровка");
    calibrationDialogWidget->setFrameStyleGeometry(styleFrame->x()-10,
                                                   styleFrame->y()-styleFrame->height()/2,
                                                   styleFrame->width()+20,
                                                   styleFrame->height()+styleFrame->height()/2);
    calibrationDialogWidget->setText("1. Сядьте прямо, на расстоянии 10см от стола,\n"
                                     "слегка наклонив голову вперёд,\n"
                                     "шея и плечи должны быть расслаблены;\n"
                                     "2. При письме локти должны лежать на столе;\n"
                                     "3. Ноги на пол надо ставить всей ступнёй,\n"
                                     "колени согнуты под углом в 90 градусов.\n"
                                     "Бёдра должны быть параллельны полу.\n"
                                     "Нажав кнопку \"Ок\" не двигайтесь 5 секунд,\n"
                                     "чтобы правильно откалибровать систему.\n"
                                     "Нажмите \"Отмена\" чтобы отменить калибровку.");
    calibrationCounterLbl->hide();
    calibrationCounterLbl->setText(QString("5"));
    calibrationCounterLbl->setGeometry(calibrationDialogWidget->getTextLabel()->geometry());
    calibrationCounterLbl->setStyleSheet(QString("background-color: rgb(250, 179, 24);"
                                                 "font: 45pt;"));
    calibrationCounterLbl->setAlignment(Qt::AlignCenter);
    askSwitchToExercise->hide();
    askSwitchToExercise->setHeader("Вопрос");
    askSwitchToExercise->setText("Хотите перейти к выполнению упражения?");
    this->setWindowIcon(QIcon(":/images/icon.png"));
    SidePanelAnimHelper *SPAnimHelper = new SidePanelAnimHelper(this, ui->sidePanelC, ui->sideBtnC);
    connect(ui->sideBtnC, &QPushButton::clicked, SPAnimHelper, &SidePanelAnimHelper::hideShowPanel);
}

void ControlWindowWidget::on_mainModeBtnC_clicked()
{
    this->hide();
    availExercWidget->setParent(this->parentWidget());
    availExercWidget->show();
    clientBlock->setParent(this->parentWidget());
    clientBlock->setGeometry(210,60,91,111);
    clientBlock->show();
    videoCamFeed->setParent(this->parentWidget());
    videoCamFeed->setWholeGeometry(60,60,111,111);
    videoCamFeed->showFullscreenBtn();
    videoCamFeed->show();

    m_isExerBlockHide = true;
    emit sig_ctrlWindowToMain();
}


void ControlWindowWidget::on_exerciseModeBtnC_clicked()
{
    this->hide();
    availExercWidget->setParent(exerciseWindow);
    availExercWidget->hide();
    askSwitchToExercise->hide();
    m_isExerBlockHide = true;
    emit sig_ctrlWindowToExercise();
}

void ControlWindowWidget::ctrlTimerSlot()
{
    ui->usefulTime_VusefulTimerC->setText(usefulTimerC->getTime());
}

void ControlWindowWidget::on_availableExercisesBtnC_clicked()
{
    if(availExercWidget->parentWidget() != this)
        return;
    if(m_isExerBlockHide)
    {
        availExercWidget->show();
        m_isExerBlockHide = false;
    }else
    {
        availExercWidget->hide();
        m_isExerBlockHide = true;
    }
}

void ControlWindowWidget::on_calibrationBtnC_clicked()
{
    if(availExercWidget->isActiveWindow())
    {
        availExercWidget->hide();
        m_isExerBlockHide = true;
    }
    calibrationDialogWidget->show();
}

void ControlWindowWidget::on_startBtnC_clicked()
{
    if(isStartBtnOn)
    {
        ui->startBtnC->setText(QString("Начать"));
        isStartBtnOn = false;
        usefulTimerC->pause();
    }
    else
    {
        ui->startBtnC->setText(QString("Отмена"));
        isStartBtnOn = true;
        usefulTimerC->resume();
    }
}

void ControlWindowWidget::on_startOkBtnClicked()
{
    if(!calibrationTimer->isActive())
        calibrationTimer->start(1000);
    calibrationDialogWidget->show();
    calibrationCounterLbl->show();
}

void ControlWindowWidget::on_startCancelBtnClicked()
{
    calibrationTimer->stop();
    calibrationCounter = 5;
    calibrationCounterLbl->setText(QString("%1").arg(calibrationCounter));
    calibrationDialogWidget->close();
    calibrationCounterLbl->close();
}

void ControlWindowWidget::decreaseCalibrationCounter()
{
    calibrationCounter--;
    calibrationCounterLbl->setText(QString("%1").arg(calibrationCounter));
    if(!calibrationCounter)
    {
        calibrationTimer->stop();
        calibrationCounter = 5;
        calibrationCounterLbl->setText(QString("%1").arg(calibrationCounter));
        calibrationDialogWidget->close();
        calibrationCounterLbl->close();
    }
}

void ControlWindowWidget::on_exerciseButtonClicked()
{
    if(availExercWidget->parentWidget() != this)
        return;
    askSwitchToExercise->show();
    askSwitchToExercise->raise();
}

void ControlWindowWidget::on_exerciseCancel()
{
    if(isStartBtnOn == true)
    {
        ui->startBtnC->setText(QString("Начать"));
        isStartBtnOn = false;
    }
}

