#ifndef MYCLASSES_H
#define MYCLASSES_H

#include <QTimer>
#include <QLabel>
#include <QString>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QtWidgets>
#include <QPushButton>


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
    QWidget *m_sidePanel;
    QPushButton *m_btn;
    QMainWindow *m_mainWind;
    bool m_isSidePanelHide = true;

public:
    SidePanelAnimHelper(QWidget *sidePanel, QPushButton *btn, QMainWindow *mainWind);

public slots:
    void hideShowPanel();

};

#endif // MYCLASSES_H
