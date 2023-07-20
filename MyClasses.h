#ifndef MYCLASSES_H
#define MYCLASSES_H

#include <QTimer>
#include <QLabel>
#include <QString>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QtWidgets>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QPixmap>

class MyTimer : public QObject
{
    Q_OBJECT
private:
    QTimer *m_timer;
    int m_hh = 0;
    int m_mm = 0;
    int m_ss = 0;

public:
    MyTimer(QObject *parent);
    ~MyTimer(){ delete m_timer; }
    QTimer *getTimer() { return m_timer; }
    void startTimer(int interval);
    void increaseTime();
    QString getTime();
};

class SidePanelAnimHelper : public QObject
{
    Q_OBJECT
private:
    QPropertyAnimation *m_panelAnim;
    QPropertyAnimation *m_btnAnim;
    QParallelAnimationGroup *m_sidePanelAnim;
    QMainWindow *m_mainWind;
    QWidget *m_sidePanel;
    QPushButton *m_btn;
    bool m_isSidePanelHide = true;

public:
    SidePanelAnimHelper(QMainWindow *mainWind, QWidget *sidePanel, QPushButton *btn);

public slots:
    void hideShowPanel();

};

class FullscreenCamAnimHelper : public QObject
{
    Q_OBJECT
private:
    QPropertyAnimation *m_fullscrnBtnAnim;
    QPropertyAnimation *m_videoCamFeedAnim;
    QPropertyAnimation *m_updCamBtnAnim;
    QParallelAnimationGroup *m_fullscreenAnim;
    QMainWindow *m_mainWind;
    QLabel *m_videoCamFeed;
    QPushButton *m_fullscrnBtn;
    QPushButton *m_updCamBtn;
    bool m_isFullscreenOn = false;
public:
    FullscreenCamAnimHelper(QMainWindow *mainWindow, QLabel *videocamFeed,
                        QPushButton *updCamBtn, QPushButton *fullscreenBtn);
public slots:
    void onOffFullscreen();
};

#endif // MYCLASSES_H
