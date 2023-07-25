#ifndef CONTROLWINDOWWIDGET_H
#define CONTROLWINDOWWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "MyClasses.h"
#include "videocamfeedwidget.h"
#include "dialogwindowwidget.h"
#include "availableexercisewidget.h"

namespace Ui {
class ControlWindowWidget;
}

class ControlWindowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWindowWidget(MyTimer* usefulTimer, AvailableExerciseWidget* availExercWidget, QWidget *parent = nullptr);
    ~ControlWindowWidget();
    void setExerciseWindow(QWidget *exerciseWindow) { this->exerciseWindow = exerciseWindow; }
    void setVideoCamFeedWidget(VideoCamFeedWidget *videocamfeedWidget) { videoCamFeed = videocamfeedWidget; }
    void setClientBlock(QWidget *clientBlockWidget) { clientBlock = clientBlockWidget; }
    void setCurStBlockStyle(QString style);
    QPushButton *getDialogOkBtn() { return calibrationDialogWidget->getOkBtn();}
    void setBackRed();
    void setNeckRed();
    void setLeftLegRed();
    void setRightLegRed();
    void setPartsWhite();

private:
    void styleHelperFunction();

signals:
    void sig_ctrlWindowToMain();
    void sig_ctrlWindowToExercise();
public slots:
    void showWindow();
    void ctrlTimerSlot();
    void on_exerciseCancel();

private slots:
    void on_mainModeBtnC_clicked();
    void on_exerciseModeBtnC_clicked();
    void on_availableExercisesBtnC_clicked();
    void on_calibrationBtnC_clicked();
    void on_startBtnC_clicked();
    void on_startOkBtnClicked();
    void on_startCancelBtnClicked();
    void decreaseCalibrationCounter();
    void on_exerciseButtonClicked();

private:
    Ui::ControlWindowWidget *ui;
    QWidget *exerciseWindow;
    AvailableExerciseWidget *availExercWidget;
    QWidget *clientBlock;
    VideoCamFeedWidget *videoCamFeed;
    MyTimer *usefulTimerC;
    bool m_isExerBlockHide = true;
    bool isStartBtnOn = false;
    dialogWindowWidget *calibrationDialogWidget;
    dialogWindowWidget *askSwitchToExercise;
    QLabel *calibrationCounterLbl;
    int calibrationCounter = 5;
    QTimer *calibrationTimer;

};

#endif // CONTROLWINDOWWIDGET_H
