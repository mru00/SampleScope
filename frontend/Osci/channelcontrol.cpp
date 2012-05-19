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


#include "channelcontrol.h"
#include "ui_channelcontrol.h"

ChannelControl::ChannelControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelControl)
{
    ui->setupUi(this);

    connect(ui->dialGain, SIGNAL(valueChanged(int)), this, SLOT(vdivValueChanged(int)));
    connect(ui->radioButtonAC, SIGNAL(toggled(bool)), this, SLOT(acdcValueChanged(bool)));
    ui->radioButtonAC->setChecked(false);
    ui->dialGain->setRange(0, DeviceConstants::Vdiv_LAST);
}

ChannelControl::~ChannelControl()
{
    delete ui;
}

void ChannelControl::setChannel(DeviceConstants::Channel_t c) {
        channel = c;
        ui->groupBox->setTitle(Device::getChannelName(channel));
}

void ChannelControl::initialEmit() {
        acdcValueChanged(ui->radioButtonAC->isChecked());
        vdivValueChanged(ui->dialGain->value());
}

void ChannelControl::acdcValueChanged(bool acdc) {
        emit acdcSelected(acdc ? DeviceConstants::ACDC_AC : DeviceConstants::ACDC_DC);
}

void ChannelControl::vdivValueChanged(int value_) {

        DeviceConstants::VdivValues_t value = (DeviceConstants::VdivValues_t)value_;
        QString label = Device::getVdivLabel(value);

        ui->labelGain->setText(label);
        emit vdivSelected(value);
}
