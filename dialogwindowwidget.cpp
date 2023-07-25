#include "dialogwindowwidget.h"
#include "ui_dialogwindowwidget.h"

dialogWindowWidget::dialogWindowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialogWindowWidget)
{
    ui->setupUi(this);
}

dialogWindowWidget::~dialogWindowWidget()
{
    delete ui;
}

void dialogWindowWidget::setText(QString text)
{
    ui->dialogText->setText(text);
}

QPushButton *dialogWindowWidget::getOkBtn()
{
    return ui->dialogOkBtn;
}

QPushButton *dialogWindowWidget::getCancelBtn()
{
    return ui->dialogCancelBtn;
}

QLabel *dialogWindowWidget::getTextLabel()
{
    return ui->dialogText;
}

QFrame *dialogWindowWidget::getFrameStyle()
{
    return ui->styleFrame;
}

void dialogWindowWidget::OffCancelBtn()
{
    ui->dialogCancelBtn->hide();
}

void dialogWindowWidget::setFrameStyleGeometry(int ax, int ay, int aw, int ah)
{
    ui->styleFrame->setGeometry(ax,ay,aw,ah);
    ui->dialogheaderFrame->setGeometry(0,
                                       0,
                                       aw,
                                       ui->dialogheaderFrame->height());
    ui->dialogText->setGeometry(10,
                                50,
                                aw-20+1,
                                ah-100+1);
    ui->dialogOkBtn->setGeometry(ui->styleFrame->width()-130,
                                 ui->styleFrame->height()-40,
                                 ui->dialogOkBtn->width(),
                                 ui->dialogOkBtn->height());
    ui->dialogCancelBtn->setGeometry(ui->styleFrame->width()-90,
                                     ui->styleFrame->height()-40,
                                     ui->dialogCancelBtn->width(),
                                     ui->dialogCancelBtn->height());
}

void dialogWindowWidget::setHeader(QString headerText)
{
    ui->dialogheader->setText(headerText);
}

void dialogWindowWidget::on_dialogCancelBtn_clicked()
{
    this->close();
}


void dialogWindowWidget::on_dialogOkBtn_clicked()
{
    this->close();
}

