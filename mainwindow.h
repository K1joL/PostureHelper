#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QProcess>
#include <QThread>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QCloseEvent>

#include "mjpegstreamer.h"
#include "MyClasses.h"
#include "controlwindowwidget.h"
#include "exercisewindowwidget.h"
#include "availableexercisewidget.h"
#include "videocamfeedwidget.h"
#include "clientinfowidget.h"
#include "dialogwindowwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void closeEvent(QCloseEvent *event) override;
    void styleHelperFunction();
    void connectProcessSignals();
private slots:
    void genTimerSlot();
    void usefulTimerSlot();
    void increasePointsForTime();
    void decreasePoint();
    //buttons
    void updateCamBtnclicked();
    void on_controlModeBtn_clicked();
    void on_exerciseModeBtn_clicked();
    //process signal
    void on_exerciseReady();
    void on_exerciseNotReady();
    void on_exerciseDid();
    void on_errorNeck();
    void on_errorBack();
    void on_errorLLeg();
    void on_errorRLeg();
    void on_errorNone();
    void on_completeExercise();
    void on_CtrlWindDialogOkBtnClicked();
    void on_startExerciseClicked();
    void on_startExerciseStand();
    void on_ExerWindDialogStartCancelClicked();
    void on_skeletonHide();
    void on_skeletonShow();
    void on_exerciseButtonClicked();
signals:
    void camStreaming();
private:
    Ui::MainWindow *ui;
    AvailableExerciseWidget *availExercWidget = nullptr;
    VideoCamFeedWidget *videoCamWidget;
    ClientInfoWidget *clientBlock;
    dialogWindowWidget *askSwitchToExercise;

    ControlWindowWidget *controlWindow = nullptr;
    ExerciseWindowWidget *exerciseWindow = nullptr;

    MyTimer *sessionTimer;
    MyTimer *usefulTimer;
    SidePanelAnimHelper *SPAnimHelper;
    FullscreenCamAnimHelper *FSAnimHelper;
    UpdateCamButton *updateCamButton;

    bool isPostureCorrect = false;
    bool isSidePanelHide = true;
    int posture_points = 0;
    int saldoPoints = 0;

    QProcess *modelProcess;
    MJPEGStreamer *streamCam;
    MessageSocket *messageSocket;
};
#endif // MAINWINDOW_H
