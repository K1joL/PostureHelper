#ifndef EXERCISEWINDOWWIDGET_H
#define EXERCISEWINDOWWIDGET_H

#include <QWidget>

#include "MyClasses.h"
#include "videocamfeedwidget.h"
#include "dialogwindowwidget.h"
#include "availableexercisewidget.h"
#include "exerciseobject.h"

namespace Ui {
class ExerciseWindowWidget;
}

class ExerciseWindowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExerciseWindowWidget(MyTimer* usefulTimer, AvailableExerciseWidget* availExercWidget, QWidget *parent = nullptr);
    ~ExerciseWindowWidget();
    void setControlWindow(QWidget *controlWindow) { this->controlWindow = controlWindow; }
    void setVideoCamFeedWidget(VideoCamFeedWidget *videocamfeedWidget) { videoCamFeed = videocamfeedWidget; }
    void setClientBlock(QWidget *clientBlockWidget) { clientBlock = clientBlockWidget; }
    void decreaseRepCounter();
    void showDialogNotReady() { dialogNotReadyWidget->show();}
    QPushButton *getDialogStartCancelBtn() { return dialogStartWidget->getCancelBtn(); }
    ExerciseObject *getCurrentExercise() { return currentExercise; }
private:
    void styleHelperFunction();

signals:
    void sig_exerciseWindowToMain();
    void sig_exerciseWindowToCtrl();
    void sig_startExerciseClicked();
    void sig_exerciseCompleted();
    void sig_cancelExerciseClicked();
    void sig_startExerciseStand();

public slots:
    void showWindow();
    void exerTimerSlot();
    void on_startOkBtnClicked();
    void on_startCancelBtnClicked();
    void decreaseStartCounter();
    void on_chooseExercise();
private slots:
    void on_mainModeBtnE_clicked();
    void on_controlModeBtnE_clicked();
    void on_availableExerecisesBtnE_clicked();
    void on_startBtnE_clicked();
    void on_instructionBtnE_clicked();
private:
    Ui::ExerciseWindowWidget *ui;
    QWidget *controlWindow;
    AvailableExerciseWidget *availExercWidget;
    QWidget *clientBlock;
    VideoCamFeedWidget *videoCamFeed;
    MyTimer *usefulTimerE;
    int repetitionCounter = 0;
    bool isExerBlockHide = true;
    bool isStartOn = false;
    dialogWindowWidget *instructionWidget;
    dialogWindowWidget *dialogNotReadyWidget;
    dialogWindowWidget *dialogStartWidget;
    //dialog window timer
    QTimer *startTimer;
    QLabel *startCounterLbl;
    int startCounter = 15;
    ExerciseObject *currentExercise = nullptr;
};

#endif // EXERCISEWINDOWWIDGET_H
