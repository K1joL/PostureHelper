#ifndef AVAILABLEEXERCISEWIDGET_H
#define AVAILABLEEXERCISEWIDGET_H

#include <QWidget>
#include <QList>

#include "exerciseobject.h"

namespace Ui {
class AvailableExerciseWidget;
}

class AvailableExerciseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AvailableExerciseWidget(QWidget *parent = nullptr);
    ~AvailableExerciseWidget();
    void addExercise(ExerciseObject* exercise);
    ExerciseObject *getCurrentExercise() const;

signals:
    void sig_exerciseButtonClicked();

public slots:
    void exerciseBtnClicked();

private:
    Ui::AvailableExerciseWidget *ui;
    QList<ExerciseObject*> *exerciseList;
    ExerciseObject *currentExercise = nullptr;
    int m_offset;
};

#endif // AVAILABLEEXERCISEWIDGET_H
