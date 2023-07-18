#include "MyClasses.h"

MyTimer::MyTimer(QObject *parent)
{
    m_timer = new QTimer(parent);
}

void MyTimer::startTimer(int interval)
{
    m_timer->start(interval);
}

void MyTimer::increaseTime()
{
    m_ss++;
    if(m_ss >= 60)
    {
        m_ss = 0;
        m_mm++;
    }
    if(m_mm >= 60)
    {
        m_mm = 0;
        m_hh++;
    }
}

QString MyTimer::getTime()
{
    return QString("%1").arg(m_hh, 2, 10, QChar('0')) + ":" +
           QString("%1").arg(m_mm, 2, 10, QChar('0')) + ":" +
           QString("%1").arg(m_ss, 2, 10, QChar('0'));
}

SidePanelAnimHelper::SidePanelAnimHelper(QWidget *sidePanel, QPushButton *btn, QMainWindow *mainWind)
    : m_sidePanel(sidePanel), m_btn(btn), m_mainWind(mainWind)
{

    if(m_sidePanel && m_btn)
    {
        //hide panel
        m_sidePanel->move(m_mainWind->width(),
                            m_sidePanel->geometry().top());
        m_btn->move(m_sidePanel->geometry().left()-m_btn->width()+1,
                            m_btn->geometry().top());
        m_panelAnim = new QPropertyAnimation(m_sidePanel, "geometry");
        m_btnAnim = new QPropertyAnimation(m_btn, "geometry");
        m_sidePanelAnim = new QParallelAnimationGroup;
        m_sidePanelAnim->addAnimation(m_panelAnim);
        m_sidePanelAnim->addAnimation(m_btnAnim);
        m_panelAnim->setDuration(300);
        m_btnAnim->setDuration(300);
    }
}

void SidePanelAnimHelper::hideShowPanel()
{
    int aleft = 0;
    if(m_isSidePanelHide)
    {
        aleft = m_mainWind->rect().width()-m_sidePanel->geometry().width()-49;
        m_isSidePanelHide = false;
    }
    else
    {
        aleft = m_mainWind->rect().width();
        m_isSidePanelHide = true;
    }
    m_panelAnim->setStartValue(
                QRect(m_sidePanel->geometry().left(),
                      m_sidePanel->geometry().top(),
                      m_sidePanel->width(),
                      m_sidePanel->height()));
    m_panelAnim->setEndValue(
                QRect(aleft,
                      m_sidePanel->geometry().top(),
                      m_sidePanel->width(),
                      m_sidePanel->height()));
    m_btnAnim->setStartValue(
                QRect(m_btn->geometry().left(),
                      m_btn->geometry().top(),
                      m_btn->width(),
                      m_btn->height()));
    m_btnAnim->setEndValue(
                QRect(m_panelAnim->endValue().toRect().left()-m_btn->width()+1,
                      m_btn->geometry().top(),
                      m_btn->geometry().width(),
                      m_btn->geometry().height()));
    m_sidePanelAnim->start();
}
