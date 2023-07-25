#ifndef VIDEOCAMFEEDWIDGET_H
#define VIDEOCAMFEEDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMovie>

#include "mjpegstreamer.h"
#include "MyClasses.h"

namespace Ui {
class VideoCamFeedWidget;
}

class VideoCamFeedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCamFeedWidget(QString url, QWidget *parent = nullptr);
    ~VideoCamFeedWidget();
    void startVideoStream();
    void stopVideoStream();
    MJPEGStreamer *getVideoStreamer();
    QPushButton *getUpdateButton();
    QPushButton *getFullscreenButton();
    void setWholeGeometry(int ax, int ay, int width, int height);
    void hideFullscreenBtn();
    void showFullscreenBtn();

signals:
    void sig_SkeletonShow();
    void sig_SkeletonHide();

private slots:
    void on_hideShowSkeletonBtn_clicked();

private:
    Ui::VideoCamFeedWidget *ui;
    QMovie *m_updateCamBtnAnim;
    FullscreenCamAnimHelper* FSAnimHelper;
    bool isSkeletonHide = true;
};

#endif // VIDEOCAMFEEDWIDGET_H
