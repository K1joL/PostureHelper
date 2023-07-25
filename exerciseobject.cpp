#include "exerciseobject.h"
#include "ui_exerciseobject.h"

ExerciseObject::ExerciseObject(QString codeName, QString name, int count, int cooldown, int points, QString instruction, QWidget *parent)
    : m_codeName(codeName),
      m_name(name),
      m_count(count),
      m_cooldown(cooldown),
      m_points(points),
      m_instruction(instruction),
      QWidget(parent),
    ui(new Ui::ExerciseObject)
{
    ui->setupUi(this);
    ui->btn_exerc->setText(name);
    ui->count_exerc->setText(QString("%1").arg(count));
    ui->cd_exerc->setText(QString("%1").arg(cooldown));
    ui->ppts_exerc->setText(QString("%1").arg(points));
    m_CDTimer = new QTimer;
}

ExerciseObject::~ExerciseObject()
{
    delete ui;
}

QPushButton *ExerciseObject::getButton()
{
    return ui->btn_exerc;
}

int ExerciseObject::getPoints() const
{
    return m_points;
}

int ExerciseObject::getCooldown() const
{
    return m_cooldown;
}

int ExerciseObject::getCount() const
{
    return m_count;
}

const QString &ExerciseObject::getName() const
{
    return m_name;
}

void ExerciseObject::complete()
{
    m_remainingTime = m_cooldown;
    //TODO: make disable button system while m_remainingTime != 0;
}

const QString &ExerciseObject::getInstruction() const
{
    return m_instruction;
}

const QString &ExerciseObject::getCodeName() const
{
    return m_codeName;
}
