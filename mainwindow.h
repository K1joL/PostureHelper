#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QProcess>

#include "MyClasses.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void timerSlot();
    void on_actionRun_model_triggered();
    void on_actionStart_session_triggered();

private:
    Ui::MainWindow *ui;
    MyTimer *SessionTimer;
    QPropertyAnimation *sidePanelAnim;
    QPropertyAnimation *sidePanelBtnAnim;
    QParallelAnimationGroup *sidePanelGroupAnim;
    bool isSidePanelHide = true;
    int posture_points;
    QProcess *modelProcess;
};
#endif // MAINWINDOW_H
