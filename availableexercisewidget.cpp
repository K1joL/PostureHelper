#include "availableexercisewidget.h"
#include "ui_availableexercisewidget.h"

AvailableExerciseWidget::AvailableExerciseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AvailableExerciseWidget)
{
    ui->setupUi(this);
    exerciseList = new QList<ExerciseObject*>;
    m_offset = 10;
}

AvailableExerciseWidget::~AvailableExerciseWidget()
{
    delete ui;
}

void AvailableExerciseWidget::addExercise(ExerciseObject* exercise)
{
    exercise->setParent(ui->Exc_ListFrame);
    connect(exercise->getButton(), &QPushButton::clicked,
            this, &AvailableExerciseWidget::exerciseBtnClicked);
    exerciseList->append(exercise);
    exercise->setGeometry(0, m_offset,
                      exercise->width(),
                      exercise->height());
    m_offset += 10 + exercise->height();

}

void AvailableExerciseWidget::exerciseBtnClicked()
{
    currentExercise = dynamic_cast<ExerciseObject*>(QObject::sender()->parent());
    emit sig_exerciseButtonClicked();
}

ExerciseObject *AvailableExerciseWidget::getCurrentExercise() const
{
    return currentExercise;
}
