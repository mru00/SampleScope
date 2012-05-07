#include "timecontrol.h"
#include "ui_timecontrol.h"

TimeControl::TimeControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeControl)
{
    ui->setupUi(this);
    connect(ui->dialDelay, SIGNAL(valueChanged(int)), this, SLOT(delayvc(int)));
    ui->dialDelay->setRange(0, DeviceConstants::Tdiv_LAST);
}

TimeControl::~TimeControl()
{
    delete ui;
}

void TimeControl::initialEmit() {
    emit delayvc(ui->dialDelay->value());
}

void TimeControl::delayvc(int value_) {
    const DeviceConstants::TdivValues_t val = (DeviceConstants::TdivValues_t)value_;
    const QString label = Device::getTdivLabel(val);
    const QString unit = Device::getTdivUnit(val);
    ui->labelUnit->setText(unit);
    ui->labelTdiv->setText(label);
    emit TdivChanged(val);
}
