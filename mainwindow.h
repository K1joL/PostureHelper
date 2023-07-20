#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QProcess>
#include <QTcpSocket>
#include <QThread>

#include "mjpegstreamer.h"
#include "MyClasses.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void timerSlot();
    void on_actionRun_model_triggered();
    void on_actionStart_session_triggered();
    void on_updateCamBtn_clicked();
    void styleHelperFunction();
    void on_actionStop_Camera_triggered();

signals:
    void camStreaming();
private:
    Ui::MainWindow *ui;
    MyTimer *sessionTimer;
    SidePanelAnimHelper *SPAnimHelper;
    FullscreenCamAnimHelper *FSAnimHelper;
    QMovie *updateCamBtnAnim;

    bool isSidePanelHide = true;
    int posture_points;
    QProcess *modelProcess;

    MJPEGStreamer *streamCam;
};
#endif // MAINWINDOW_H
