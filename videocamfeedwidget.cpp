#include "videocamfeedwidget.h"
#include "ui_videocamfeedwidget.h"

VideoCamFeedWidget::VideoCamFeedWidget(QString url, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCamFeedWidget)
{
    ui->setupUi(this);
    ui->videocam_feedW->set_url(url);
    //Animation updateCamBtn
    QIcon updCamBtnIcon(":/images/iconupdbtn.png");
    ui->updateCamBtnW->setIconSize(QSize(48,48));
    ui->updateCamBtnW->setIcon(updCamBtnIcon);
    m_updateCamBtnAnim = new QMovie(":/images/gifupdbtn.gif");
    connect(m_updateCamBtnAnim, &QMovie::frameChanged, [=]{
      ui->updateCamBtnW->setIcon(m_updateCamBtnAnim->currentPixmap());
    });
    connect(ui->videocam_feedW, &MJPEGStreamer::streaming, [=]{
        m_updateCamBtnAnim->stop();
        ui->updateCamBtnW->setIcon(updCamBtnIcon);
        ui->updateCamBtnW->hide();
    });
    connect(ui->videocam_feedW, &MJPEGStreamer::disconnected, [=]{
        ui->updateCamBtnW->show();
    });
    //Animation fullscreen camera
    //FS - Fullcreen
    FSAnimHelper = new FullscreenCamAnimHelper(this, ui->videocam_feedW, ui->updateCamBtnW,
                                               ui->fullscrnBtnW);
    connect(ui->fullscrnBtnW, &QPushButton::clicked,
            FSAnimHelper, &FullscreenCamAnimHelper::onOffFullscreen);
}

VideoCamFeedWidget::~VideoCamFeedWidget()
{
    delete ui;
}

void VideoCamFeedWidget::startVideoStream()
{
    m_updateCamBtnAnim->start();
    ui->videocam_feedW->start();
}

void VideoCamFeedWidget::stopVideoStream()
{
    ui->videocam_feedW->stop();
}

MJPEGStreamer *VideoCamFeedWidget::getVideoStreamer()
{
    return ui->videocam_feedW;
}

QPushButton *VideoCamFeedWidget::getUpdateButton()
{
    return ui->updateCamBtnW;
}

QPushButton *VideoCamFeedWidget::getFullscreenButton()
{
    return ui->fullscrnBtnW;
}

void VideoCamFeedWidget::setWholeGeometry(int ax, int ay, int awidth, int aheight)
{
    this->setGeometry(ax, ay, awidth, aheight);
    ui->videocam_feedW->resize(awidth, aheight);
    ui->updateCamBtnW->resize(awidth, aheight);
}

void VideoCamFeedWidget::hideFullscreenBtn()
{
    ui->fullscrnBtnW->hide();
}

void VideoCamFeedWidget::showFullscreenBtn()
{
    ui->fullscrnBtnW->show();
}


void VideoCamFeedWidget::on_hideShowSkeletonBtn_clicked()
{
    if(isSkeletonHide)
    {
        isSkeletonHide = false;
        emit sig_SkeletonShow();
    }else
    {
        isSkeletonHide = true;
        emit sig_SkeletonHide();
    }
}

