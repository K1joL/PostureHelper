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

SidePanelAnimHelper::SidePanelAnimHelper(QMainWindow *mainWind, QWidget *sidePanel, QPushButton *btn)
    : m_mainWind(mainWind), m_sidePanel(sidePanel), m_btn(btn)
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
        //show panel
        aleft = m_mainWind->rect().width()-m_sidePanel->geometry().width()-49;
        m_isSidePanelHide = false;
    }
    else
    {
        //hide panel
        aleft = m_mainWind->rect().width();
        m_isSidePanelHide = true;
    }
//    m_panelAnim->setStartValue(
//                QRect(m_sidePanel->geometry().left(),
//                      m_sidePanel->geometry().top(),
//                      m_sidePanel->width(),
//                      m_sidePanel->height()));
    m_panelAnim->setEndValue(
                QRect(aleft,
                      m_sidePanel->y(),
                      m_sidePanel->width(),
                      m_sidePanel->height()));
//    m_btnAnim->setStartValue(
//                QRect(m_btn->x(),
//                      m_btn->y(),
//                      m_btn->width(),
//                      m_btn->height()));
    m_btnAnim->setEndValue(
                QRect(m_panelAnim->endValue().toRect().left()-m_btn->width()+1,
                      m_btn->geometry().top(),
                      m_btn->geometry().width(),
                      m_btn->geometry().height()));
    m_sidePanelAnim->start();
}

FullscreenCamAnimHelper::FullscreenCamAnimHelper(QMainWindow *mainWindow, QLabel *videocamFeed,
                                                 QPushButton *updateCamBtn, QPushButton *fullscreenBtn)
    : m_mainWind(mainWindow), m_videoCamFeed(videocamFeed), m_updCamBtn(updateCamBtn), m_fullscrnBtn(fullscreenBtn)
{
    if(m_videoCamFeed && m_updCamBtn && m_fullscrnBtn)
    {
        m_videoCamFeedAnim = new QPropertyAnimation(m_videoCamFeed, "size");
        m_updCamBtnAnim = new QPropertyAnimation(m_updCamBtn, "size");
        m_fullscrnBtnAnim = new QPropertyAnimation(m_fullscrnBtn, "geometry");
        m_fullscreenAnim = new QParallelAnimationGroup;
        //ANIMATION DURATION
        m_updCamBtnAnim->setDuration(500);
        m_fullscrnBtnAnim->setDuration(500);
        m_videoCamFeedAnim->setDuration(500);
        m_fullscreenAnim->addAnimation(m_updCamBtnAnim);
        m_fullscreenAnim->addAnimation(m_fullscrnBtnAnim);
        m_fullscreenAnim->addAnimation(m_videoCamFeedAnim);
    }
    else
    {
        qDebug() << "One or more arguments are nullptr!";
    }
}

void FullscreenCamAnimHelper::onOffFullscreen()
{
    int widthOffset = m_videoCamFeed->width();
    int heightOffset = m_videoCamFeed->height();
    int fullscrnBtnLeft = 0;
    int fullscrnBtnTop = 0;
    if(m_isFullscreenOn)
    {
        widthOffset -= 560;
        heightOffset -= 370;
        fullscrnBtnLeft = widthOffset - m_fullscrnBtn->width();
        fullscrnBtnTop = heightOffset - m_fullscrnBtn->height();
        m_isFullscreenOn = false;
    }else
    {
        widthOffset += 560;
        heightOffset += 370;
        fullscrnBtnLeft = widthOffset - m_fullscrnBtn->width();
        fullscrnBtnTop = heightOffset - m_fullscrnBtn->height();
        m_isFullscreenOn = true;
    }

//    m_videoCamFeedAnim->setStartValue(QSize(m_videoCamFeed->width(),m_videoCamFeed->height()));
    m_videoCamFeedAnim->setEndValue(QSize(widthOffset,heightOffset));
//    m_updCamBtnAnim->setStartValue(QSize(m_updCamBtn->width(),m_updCamBtn->height()));
    m_updCamBtnAnim->setEndValue(QSize(widthOffset,heightOffset));
//    m_fullscrnBtnAnim->setStartValue(QRect(m_fullscrnBtn->x(),
//                                           m_fullscrnBtn->y(),
//                                           m_fullscrnBtn->width(),
//                                           m_fullscrnBtn->height()));
    m_fullscrnBtnAnim->setEndValue(QRect(fullscrnBtnLeft,
                                         fullscrnBtnTop,
                                         m_fullscrnBtn->width(),
                                         m_fullscrnBtn->height() ));
    m_fullscreenAnim->start();
}
