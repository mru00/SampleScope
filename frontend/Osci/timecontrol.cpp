#include "timecontrol.h"
#include "ui_timecontrol.h"

TimeControl::TimeControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeControl)
{
    ui->setupUi(this);
    connect(ui->dialDelay, SIGNAL(valueChanged(int)), this, SLOT(delayvc(int)));
}

TimeControl::~TimeControl()
{
    delete ui;
}

void TimeControl::initialEmit() {
        emit delayValueChanged(ui->dialDelay->value());
}

void TimeControl::delayvc(int value) {
        emit delayValueChanged(value);
}
