#ifndef MYCLASSES_H
#define MYCLASSES_H

#include <QTimer>
#include <QLabel>
#include <QString>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QtWidgets>
#include <QPushButton>
#include <QPixmap>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QMap>

#include "mjpegstreamer.h"

class MyTimer : public QObject
{
    Q_OBJECT

public:
    MyTimer(QObject *parent = nullptr);
    virtual ~MyTimer(){ delete m_timer; }
    QTimer *getTimer() { return m_timer; }
    void startTimer(int interval = 0);
    void stopTimer() { m_timer->stop(); }
    void pause();
    void resume();
    void increaseTime();
    QString getTime();
private:
    void changeInterval(int newInterval);
    void changeSingleShot(bool newSingleShot);
    void reset();
signals:
    void sig_increasePoints();

private:
    QTimer *m_timer;
    int remaining;
    int m_oldInterval;
    bool m_singleShot = false;
    int m_hh = 0;
    int m_mm = 0;
    int m_ss = 0;
};

class SidePanelAnimHelper : public QObject
{
    Q_OBJECT
private:
    QPropertyAnimation *m_panelAnim;
    QPropertyAnimation *m_btnAnim;
    QParallelAnimationGroup *m_sidePanelAnim;
    QWidget *m_mainWind;
    QWidget *m_sidePanel;
    QPushButton *m_btn;
    bool m_isSidePanelHide = true;

public:
    SidePanelAnimHelper(QWidget *mainWind, QWidget *sidePanel, QPushButton *btn);
    virtual ~SidePanelAnimHelper(){}

public slots:
    void hideShowPanel();

};

class FullscreenCamAnimHelper : public QObject
{
    Q_OBJECT
public:
    FullscreenCamAnimHelper(QWidget *mainWidget, QWidget *videocamFeed,
                        QPushButton *updCamBtn, QPushButton *fullscreenBtn);
    virtual ~FullscreenCamAnimHelper(){}

public slots:
    void onOffFullscreen();

private:
    QPropertyAnimation *m_mainWidgetAnim;
    QPropertyAnimation *m_videoCamFeedAnim;
    QPropertyAnimation *m_updCamBtnAnim;
    QPropertyAnimation *m_fullscrnBtnAnim;
    QParallelAnimationGroup *m_fullscreenAnim;
    QWidget *m_mainWidget;
    QWidget *m_videoCamFeed;
    QPushButton *m_fullscrnBtn;
    QPushButton *m_updCamBtn;
    bool m_isFullscreenOn = false;
};

class ExerciseBlockAnimHelper : public QObject
{
    Q_OBJECT
public:
    ExerciseBlockAnimHelper(QWidget *avalExerWidget);
    virtual ~ExerciseBlockAnimHelper(){}
public slots:
    void hideShowBlock();

private:
    QPropertyAnimation m_blockAnim;
    QWidget *avalExerWidget;
};

class UpdateCamButton : public QObject
{
    Q_OBJECT
public:
    UpdateCamButton(QPushButton *updateCamButton, MJPEGStreamer *camWidget);
    void startAnim();
private:
    QMovie *m_updateCamBtnAnim;
};

class MessageSocket : public QUdpSocket
{
    Q_OBJECT
public:
    MessageSocket(QHostAddress address, quint16 portIn, quint16 portTo);
    void send(QString message);
    void processData(const QString message);
    void initMaps();
signals:
    void sig_error_back_received();
    void sig_error_neck_received();
    void sig_error_lLeg_received();
    void sig_error_rLeg_received();
    void sig_error_none_received();
    void sig_exercise_did_received();
    void sig_exercise_start_received();
    void sig_exercise_ready_received();
    void sig_exercise_notReady_received();
public slots:
    void read();

private:
    enum cmdEnum{ Error,
                  Ex,
                  MAX_CMD};
    enum errorValueEnum{ None,
                         Neck,
                         Back,
                         LeftLeg,
                         RightLeg,
                         MAX_ERROR_VALUE};
    enum exValueEnum{ Start,
                      Ready,
                      NotReady,
                      Did,
                      MAX_EX_VALUE};

    QUdpSocket *m_socket;
    QHostAddress m_address;
    quint16 m_portIn;
    quint16 m_portTo;
    QMap<QString, int> m_cmdMap;
    QMap<QString, int> m_errorValueMap;
    QMap<QString, int> m_exValueMap;
};

#endif // MYCLASSES_H
