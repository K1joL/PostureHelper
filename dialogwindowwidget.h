#ifndef DIALOGWINDOWWIDGET_H
#define DIALOGWINDOWWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class dialogWindowWidget;
}

class dialogWindowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dialogWindowWidget(QWidget *parent = nullptr);
    ~dialogWindowWidget();
    void setText(QString text);
    void setHeader(QString headerText);
    QPushButton *getOkBtn();
    QPushButton *getCancelBtn();
    QLabel *getTextLabel();
    QFrame *getFrameStyle();
    void OffCancelBtn();
    void setFrameStyleGeometry(int ax, int ay, int aw, int ah);
private slots:
    void on_dialogCancelBtn_clicked();
    void on_dialogOkBtn_clicked();

private:
    Ui::dialogWindowWidget *ui;
};

#endif // DIALOGWINDOWWIDGET_H
