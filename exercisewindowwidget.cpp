#include "exercisewindowwidget.h"
#include "ui_exercisewindowwidget.h"

ExerciseWindowWidget::ExerciseWindowWidget(MyTimer* usefulTimer, AvailableExerciseWidget* availExercWidget, QWidget *parent) :
    usefulTimerE(usefulTimer),
    availExercWidget(availExercWidget),
    QWidget(parent),
    ui(new Ui::ExerciseWindowWidget)
{
    ui->setupUi(this);
    dialogNotReadyWidget = new dialogWindowWidget(this);
    instructionWidget = new dialogWindowWidget(this);
    dialogStartWidget = new dialogWindowWidget(this);
    startCounterLbl = new QLabel(dialogStartWidget);
    startTimer = new QTimer(this);
    connect(dialogStartWidget->getOkBtn(), &QPushButton::clicked,
            this, &ExerciseWindowWidget::on_startOkBtnClicked);
    connect(dialogStartWidget->getCancelBtn(), &QPushButton::clicked,
            this, &ExerciseWindowWidget::on_startCancelBtnClicked);
    connect(startTimer, &QTimer::timeout,
            this, &ExerciseWindowWidget::decreaseStartCounter);
    connect(availExercWidget, &AvailableExerciseWidget::sig_exerciseButtonClicked,
            this, &ExerciseWindowWidget::on_chooseExercise);
    styleHelperFunction();
    connect(usefulTimer->getTimer(), SIGNAL(timeout()),
            this, SLOT(exerTimerSlot()));
}

ExerciseWindowWidget::~ExerciseWindowWidget()
{
    delete ui;
}

void ExerciseWindowWidget::decreaseRepCounter()
{
    repetitionCounter--;
    ui->ChExBlock_VrepetitionE->setText(QString("%1").arg(repetitionCounter));
    if(!repetitionCounter)
    {
        repetitionCounter = availExercWidget->getCurrentExercise()->getCount();
        ui->ChExBlock_VrepetitionE->setText(QString("%1").arg(repetitionCounter));
        emit sig_exerciseCompleted();
    }
}

void ExerciseWindowWidget::showWindow()
{
    this->show();
    videoCamFeed->setParent(ui->videofeed_frameE);
    videoCamFeed->setWholeGeometry(10,10,611,481);
    videoCamFeed->show();
    clientBlock->setParent(ui->MainInfoFrameE);
    clientBlock->setGeometry(10,10,91,111);
    clientBlock->show();
}

void ExerciseWindowWidget::styleHelperFunction()
{
    this->hide();
    dialogNotReadyWidget->hide();
    dialogNotReadyWidget->setHeader(QString("Вы не готовы!"));
    dialogNotReadyWidget->setText(QString("Выполнение упражнения не будет продолжено.\n"
                                          "Возможно, вы не успели встать в исходную позицию\n"
                                          "или камера вас не видит.\n"
                                          "Попробуйте ещё раз!"));
    dialogNotReadyWidget->setGeometry(0, 0,
                              dialogNotReadyWidget->width(),
                              dialogNotReadyWidget->height());
    instructionWidget->hide();
    QFrame *instructionStyleFrame = instructionWidget->getFrameStyle();
    instructionWidget->OffCancelBtn();
    instructionWidget->setHeader(QString("Инструкция к упражнению"));
    instructionWidget->setFrameStyleGeometry(instructionStyleFrame->x()-50,
                                   instructionStyleFrame->y()-instructionStyleFrame->height()/4,
                                   instructionStyleFrame->width()+100,
                                   instructionStyleFrame->height()+instructionStyleFrame->height()/3);
    dialogStartWidget->hide();
    dialogStartWidget->setHeader(QString("Начало упражнения"));
    dialogStartWidget->setText(QString("Сейчас начнётся выполнение упражнения.\n"
                                       "Нажав кнопку \"Ок\" у вас будет 15 сек,"
                                       "\nчтобы встать в исходную позицию.\n"
                                       "Чтобы отказаться нажмите \"Отмена\".\n"
                                       "Когда будете готовы нажмите \"Ок\".\n"));
    dialogStartWidget->setGeometry(0, 0,
                              dialogStartWidget->width(),
                              dialogStartWidget->height());
    startCounterLbl->hide();
    startCounterLbl->setText(QString("15"));
    startCounterLbl->setGeometry(dialogStartWidget->getTextLabel()->x(),
                                 dialogStartWidget->getTextLabel()->y(),
                                 dialogStartWidget->getTextLabel()->width(),
                                 dialogStartWidget->getTextLabel()->height());
    startCounterLbl->setStyleSheet(QString("font: 45pt;"));
    startCounterLbl->setAlignment(Qt::AlignCenter);
    this->setWindowIcon(QIcon(":/images/icon.png"));
    SidePanelAnimHelper *SPAnimgHelper = new SidePanelAnimHelper(this, ui->sidePanelE, ui->sideBtnE);
    connect(ui->sideBtnE, &QPushButton::clicked, SPAnimgHelper, &SidePanelAnimHelper::hideShowPanel);

}

void ExerciseWindowWidget::on_mainModeBtnE_clicked()
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
    isExerBlockHide = true;
    emit sig_exerciseWindowToMain();
}


void ExerciseWindowWidget::on_controlModeBtnE_clicked()
{
    this->hide();
    availExercWidget->setParent(controlWindow);
    availExercWidget->hide();
    isExerBlockHide = true;
    emit sig_exerciseWindowToCtrl();
}

void ExerciseWindowWidget::exerTimerSlot()
{
    ui->usefulTime_VusefulTimerE->setText(usefulTimerE->getTime());
}

void ExerciseWindowWidget::on_startOkBtnClicked()
{
    if(!startTimer->isActive())
        startTimer->start(1000);
    dialogStartWidget->show();
    startCounterLbl->show();
}

void ExerciseWindowWidget::on_startCancelBtnClicked()
{
    startTimer->stop();
    startCounter = 15;
    startCounterLbl->setText(QString("%1").arg(startCounter));
    dialogStartWidget->close();
    startCounterLbl->close();
    //update state start button
    ui->startBtnE->setText(QString("Начать"));
    isStartOn = false;
    emit sig_cancelExerciseClicked();
}

void ExerciseWindowWidget::decreaseStartCounter()
{
    startCounter--;
    startCounterLbl->setText(QString("%1").arg(startCounter));
    if(!startCounter)
    {
        startTimer->stop();
        startCounter = 15;
        startCounterLbl->setText(QString("%1").arg(startCounter));
        dialogStartWidget->close();
        startCounterLbl->close();
        emit sig_startExerciseStand();
    }
}

void ExerciseWindowWidget::on_chooseExercise()
{
    currentExercise = availExercWidget->getCurrentExercise();
    ui->ChExBlock_VexerciseNameE->setText(currentExercise->getName());
    repetitionCounter = currentExercise->getCount();
    ui->ChExBlock_VrepetitionE->setText(QString("%1").arg(repetitionCounter));
    emit ui->availableExerecisesBtnE->clicked();
}

void ExerciseWindowWidget::on_availableExerecisesBtnE_clicked()
{
    if(availExercWidget->parentWidget() != this)
        return;
    if(isExerBlockHide)
    {
        availExercWidget->show();
        isExerBlockHide = false;
    }else
    {
        availExercWidget->hide();
        isExerBlockHide = true;
    }
}

void ExerciseWindowWidget::on_startBtnE_clicked()
{
    if(isStartOn)
    {
        ui->startBtnE->setText(QString("Начать"));
        dialogStartWidget->close();
        isStartOn = false;
        emit sig_cancelExerciseClicked();
    }
    else
    {
        if(!availExercWidget->getCurrentExercise())
            return;
        if(availExercWidget->isActiveWindow())
        {
            availExercWidget->hide();
            isExerBlockHide = true;
        }
        ui->startBtnE->setText(QString("Отмена"));
        usefulTimerE->pause();
        dialogStartWidget->show();
        isStartOn = true;
        emit sig_startExerciseClicked();
    }
}

void ExerciseWindowWidget::on_instructionBtnE_clicked()
{
    if(!availExercWidget->getCurrentExercise())
        return;
    if(availExercWidget->isActiveWindow())
    {
        availExercWidget->hide();
        isExerBlockHide = true;
    }
    instructionWidget->setText(currentExercise->getInstruction());
    instructionWidget->show();
}

