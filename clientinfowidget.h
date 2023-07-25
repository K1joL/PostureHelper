#ifndef CLIENTINFOWIDGET_H
#define CLIENTINFOWIDGET_H

#include <QWidget>

namespace Ui {
class ClientInfoWidget;
}

class ClientInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientInfoWidget(QWidget *parent = nullptr);
    ~ClientInfoWidget();
    void addPoints(int points);
signals:
    void sig_pointsChanged();
private slots:
    void setPointLabel();

private:
    Ui::ClientInfoWidget *ui;
    int m_points;
};

#endif // CLIENTINFOWIDGET_H
