#include "clientinfowidget.h"
#include "ui_clientinfowidget.h"

ClientInfoWidget::ClientInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientInfoWidget)
{
    ui->setupUi(this);
    connect(this, &ClientInfoWidget::sig_pointsChanged,
            this, &ClientInfoWidget::setPointLabel);
}

ClientInfoWidget::~ClientInfoWidget()
{
    delete ui;
}

void ClientInfoWidget::addPoints(int points)
{
    m_points += points;
    emit sig_pointsChanged();
}

void ClientInfoWidget::setPointLabel()
{
    ui->BlockClient_Vpts->setText(QString("%1").arg(m_points));
}
