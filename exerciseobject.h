#ifndef EXERCISEOBJECT_H
#define EXERCISEOBJECT_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>

namespace Ui {
class ExerciseObject;
}

class ExerciseObject : public QWidget
{
    Q_OBJECT
public:
    explicit ExerciseObject(QString codeName, QString name, int count, int cooldown, int points, QString instruction, QWidget *parent = nullptr);
    ~ExerciseObject();

    QPushButton *getButton();
    int getPoints() const;
    int getCooldown() const;
    int getCount() const;
    const QString &getName() const;
    void complete();

    const QString &getInstruction() const;

    const QString &getCodeName() const;

private:
    Ui::ExerciseObject *ui;
    QString m_instruction;
    QString m_codeName;
    QString m_name;
    int m_count;
    int m_cooldown;
    int m_points;
    QTimer *m_CDTimer;
    int m_remainingTime = 0;
};

#endif // EXERCISEOBJECT_H
