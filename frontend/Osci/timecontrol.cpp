/*

SampleScope: A humble USB oscilloscope
Copyright (C) 2012 mru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
