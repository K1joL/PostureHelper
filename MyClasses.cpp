#include "MyClasses.h"

MyTimer::MyTimer(QObject *parent)
{
    m_timer = new QTimer(parent);
}

void MyTimer::startTimer(int interval)
{
    m_timer->start(interval);
    m_oldInterval = interval;
}

void MyTimer::pause()
{
    remaining = m_timer->remainingTime();
    m_timer->stop();
}

void MyTimer::resume()
{
    if (remaining < 0) return;
    m_timer->setInterval(remaining);
    m_timer->setSingleShot(true);
    QObject::connect(m_timer, &QTimer::timeout, this, &MyTimer::reset);
    m_timer->start();
}

void MyTimer::increaseTime()
{
    m_ss++;
    if(m_ss >= 60)
    {
        m_ss = 0;
        m_mm++;
        if(m_mm % 5 == 0)
            emit sig_increasePoints();
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

void MyTimer::changeInterval(int newInterval)
{
    m_oldInterval = newInterval;
    m_timer->setInterval(newInterval);
}

void MyTimer::changeSingleShot(bool newSingleShot)
{
    m_singleShot = newSingleShot;
    m_timer->setSingleShot(newSingleShot);
}

void MyTimer::reset()
{
    m_timer->setInterval(m_oldInterval);
    m_timer->setSingleShot(m_singleShot);
    QObject::disconnect(m_timer, &QTimer::timeout, this, &MyTimer::reset);
    if (!m_singleShot) {
        m_timer->start();
    }
}

SidePanelAnimHelper::SidePanelAnimHelper(QWidget *mainWind, QWidget *sidePanel, QPushButton *btn)
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
    }else
    {
        qDebug() << "One or more arguments are nullptr!";
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
        m_btn->setText(">");
    }
    else
    {
        //hide panel
        aleft = m_mainWind->rect().width();
        m_isSidePanelHide = true;
        m_btn->setText("<");
    }
    m_panelAnim->setEndValue(
                QRect(aleft,
                      m_sidePanel->y(),
                      m_sidePanel->width(),
                      m_sidePanel->height()));
    m_btnAnim->setEndValue(
                QRect(m_panelAnim->endValue().toRect().left()-m_btn->width()+1,
                      m_btn->geometry().top(),
                      m_btn->geometry().width(),
                      m_btn->geometry().height()));
    m_sidePanelAnim->start();
}

FullscreenCamAnimHelper::FullscreenCamAnimHelper(QWidget *mainWidget, QWidget *videocamFeed, QPushButton *updateCamBtn,
                                                 QPushButton *fullscreenBtn):
      m_mainWidget(mainWidget),
      m_videoCamFeed(videocamFeed),
      m_updCamBtn(updateCamBtn),
      m_fullscrnBtn(fullscreenBtn)
{
    int duration = 500;
    if(m_videoCamFeed && m_updCamBtn && m_fullscrnBtn)
    {
        m_mainWidgetAnim = new QPropertyAnimation(m_mainWidget, "size");
        m_videoCamFeedAnim = new QPropertyAnimation(m_videoCamFeed, "size");
        m_updCamBtnAnim = new QPropertyAnimation(m_updCamBtn, "size");
        m_fullscrnBtnAnim = new QPropertyAnimation(m_fullscrnBtn, "geometry");
        m_fullscreenAnim = new QParallelAnimationGroup;
        //ANIMATION DURATION
        m_mainWidgetAnim->setDuration(duration);
        m_videoCamFeedAnim->setDuration(duration);
        m_updCamBtnAnim->setDuration(duration);
        m_fullscrnBtnAnim->setDuration(duration);
        m_fullscreenAnim->addAnimation(m_mainWidgetAnim);
        m_fullscreenAnim->addAnimation(m_videoCamFeedAnim);
        m_fullscreenAnim->addAnimation(m_updCamBtnAnim);
        m_fullscreenAnim->addAnimation(m_fullscrnBtnAnim);
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
        widthOffset -= 561;
        heightOffset -= 371;
        fullscrnBtnLeft = widthOffset - m_fullscrnBtn->width();
        fullscrnBtnTop = heightOffset - m_fullscrnBtn->height();
        m_isFullscreenOn = false;
    }else
    {
        widthOffset += 561;
        heightOffset += 371;
        fullscrnBtnLeft = widthOffset - m_fullscrnBtn->width();
        fullscrnBtnTop = heightOffset - m_fullscrnBtn->height();
        m_isFullscreenOn = true;
    }

    m_mainWidgetAnim->setEndValue(QSize(widthOffset,heightOffset));
    m_videoCamFeedAnim->setEndValue(QSize(widthOffset,heightOffset));
    m_updCamBtnAnim->setEndValue(QSize(widthOffset,heightOffset));
    m_fullscrnBtnAnim->setEndValue(QRect(fullscrnBtnLeft,
                                         fullscrnBtnTop,
                                         m_fullscrnBtn->width(),
                                         m_fullscrnBtn->height() ));
    m_fullscreenAnim->start();
}

UpdateCamButton::UpdateCamButton(QPushButton *updateCamButton, MJPEGStreamer *camWidget)
{
    //Animation updateCamBtn
    QIcon updCamBtnIcon(":/images/iconupdbtn.png");
    updateCamButton->setIconSize(QSize(48,48));
    updateCamButton->setIcon(updCamBtnIcon);
    m_updateCamBtnAnim = new QMovie(":/images/gifupdbtn.gif");
    connect(m_updateCamBtnAnim, &QMovie::frameChanged, [=]{
      updateCamButton->setIcon(m_updateCamBtnAnim->currentPixmap());
    });
    connect(camWidget, &MJPEGStreamer::streaming, [=]{
        m_updateCamBtnAnim->stop();
        updateCamButton->setIcon(updCamBtnIcon);
        updateCamButton->hide();
    });
    connect(camWidget, &MJPEGStreamer::disconnected, [=]{
        updateCamButton->show();
    });
}

void UpdateCamButton::startAnim()
{
    m_updateCamBtnAnim->start();
}

MessageSocket::MessageSocket(QHostAddress address, quint16 portIn, quint16 portTo)
    :
      m_socket(new QUdpSocket),
      m_address(address),
      m_portIn(portIn),
      m_portTo(portTo)
{
    m_socket->bind(m_address, m_portIn);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(read()));
    initMaps();
}

void MessageSocket::read()
{
    QByteArray data;
    data.resize(m_socket->pendingDatagramSize());
    m_socket->readDatagram(data.data(), data.size(), &m_address);
    QString str(QString::fromUtf8(data));
    processData(QString(str));
}

void MessageSocket::send(QString message)
{
    m_socket->writeDatagram(message.toUtf8(), m_address, m_portTo);
}

/*
*   command structure: "cmd value ..."
*   cmd types:
*       Error
*       Ex (exercise)
*   Ex value types:
*       Ready
*       NotReady
*       Start
*       Did
*   Error value types:
*       Neck
*       Back
*       LeftLeg
*       RightLeg
*       None
*/
void MessageSocket::processData(const QString message)
{
    QStringList bufferList = message.split(" ", QString::SkipEmptyParts);
    if(bufferList.size() == 1 || bufferList.size() == 0)
        return;
    int cmd = *m_cmdMap.find(bufferList.at(0));
    //change first element to none to clean all labels first
    bufferList[0] = ("None");
    int value = 0;
    switch(cmd)
    {
        case Error:
            for(auto &listValue : bufferList)
            {
                value = *m_errorValueMap.find(listValue);
                qDebug() << value << " " << listValue;
                switch(value)
                {
                    case Neck:
                        emit sig_error_neck_received();
                        break;
                    case Back:
                        emit sig_error_back_received();
                        break;
                    case LeftLeg:
                        emit sig_error_lLeg_received();
                        break;
                    case RightLeg:
                        emit sig_error_rLeg_received();
                        break;
                    case None:
                        emit sig_error_none_received();
                        break;
                    default:
                        qDebug() << "Wrong Error command value!";
                        return;
                }
            }
            break;
        case Ex:
            value = *m_exValueMap.find(bufferList.at(1));
            switch(value)
            {
            case Start:
                emit sig_exercise_start_received();
                break;
            case Ready:
                emit sig_exercise_ready_received();
                break;
            case NotReady:
                emit sig_exercise_notReady_received();
                break;
            case Did:
                emit sig_exercise_did_received();
                break;
            default:
                qDebug() << "Wrong Ex command value!";
            }
            break;
        default:
        qDebug() << "Wrong cmd";
        return;
    }
}

void MessageSocket::initMaps()
{
    //cmd
    m_cmdMap.insert("Error", cmdEnum::Error);
    m_cmdMap.insert("Ex", cmdEnum::Ex);
    //Error value
    m_errorValueMap.insert("None", errorValueEnum::None);
    m_errorValueMap.insert("Neck", errorValueEnum::Neck);
    m_errorValueMap.insert("Back", errorValueEnum::Back);
    m_errorValueMap.insert("LeftLeg", errorValueEnum::LeftLeg);
    m_errorValueMap.insert("RightLeg", errorValueEnum::RightLeg);
    //Ex value
    m_exValueMap.insert("Start", exValueEnum::Start);
    m_exValueMap.insert("Ready", exValueEnum::Ready);
    m_exValueMap.insert("NotReady", exValueEnum::NotReady);
    m_exValueMap.insert("Did", exValueEnum::Did);
}
