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


#include "triggercontrol.h"
#include "ui_triggercontrol.h"

TriggerControl::TriggerControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerControl)
{
    ui->setupUi(this);
    connect(ui->radioButtonCh1, SIGNAL(toggled(bool)), this, SLOT(triggerCh1Toggled(bool)));
    connect(ui->radioButtonCh2, SIGNAL(toggled(bool)), this, SLOT(triggerCh2Toggled(bool)));
    connect(ui->radioButtonFree, SIGNAL(toggled(bool)), this, SLOT(triggerFreeToggled(bool)));

    connect(ui->radioButtonRising, SIGNAL(toggled(bool)), this, SLOT(triggerRisingToggled(bool)));
    connect(ui->radioButtonFalling, SIGNAL(toggled(bool)), this, SLOT(triggerFallingToggled(bool)));
    ui->radioButtonFree->setChecked(true);
    ui->radioButtonRising->setChecked(true);
}

TriggerControl::~TriggerControl()
{
    delete ui;
}

void TriggerControl::initialEmit() {
    if (ui->radioButtonCh1->isChecked()) triggerCh1Toggled(true);
    else if (ui->radioButtonCh2->isChecked()) triggerCh2Toggled(true);
    else triggerFreeToggled(true);


    if (ui->radioButtonRising->isChecked()) triggerRisingToggled(true);
    else triggerFallingToggled(true);

}

void TriggerControl::triggerCh1Toggled(bool checked) {
    if (checked) emit triggerSourceSelected(DeviceConstants::TRIGGER_CH_1);
}
void TriggerControl::triggerCh2Toggled(bool checked) {
    if (checked) emit triggerSourceSelected(DeviceConstants::TRIGGER_CH_2);
}
void TriggerControl::triggerFreeToggled(bool checked) {
    if (checked) emit triggerSourceSelected(DeviceConstants::Trigger_Free);
}


void TriggerControl::triggerRisingToggled(bool val) {
    if(val) emit triggerModeSelected(DeviceConstants::TRIGGER_RISING);
}

void TriggerControl::triggerFallingToggled(bool val) {
    if(val) emit triggerModeSelected(DeviceConstants::TRIGGER_FALLING);
}
