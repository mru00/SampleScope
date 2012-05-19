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


#include "calibdialog.h"
#include "ui_calibdialog.h"

#include "device.h"

CalibDialog::CalibDialog(Device* device,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibDialog),
    device(device)
{
    ui->setupUi(this);
    connect(ui->spinBoxDelay, SIGNAL(valueChanged(int)), this, SLOT(delayChanged(int)));
}

CalibDialog::~CalibDialog()
{
    delete ui;
}

void CalibDialog::delayChanged(int value) {
    device->setDelay(value);
}

void CalibDialog::setDelay(unsigned short value) {
    ui->spinBoxDelay->setValue(value);
}
