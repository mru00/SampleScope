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


#ifndef CHANNELCONTROL_H
#define CHANNELCONTROL_H

#include <QCheckBox>
#include <QWidget>
#include "device.h"

namespace Ui {
class ChannelControl;
}

class ChannelControl : public QWidget
{
        Q_OBJECT

public:
        explicit ChannelControl(QWidget *parent = 0);
        ~ChannelControl();

        void setChannel(DeviceConstants::Channel_t c);
        void initialEmit();
        bool isActive();

private slots:
        void vdivValueChanged(int gain);
        void acdcValueChanged(bool acdc);
signals:
        void vdivSelected(DeviceConstants::VdivValues_t code);
        void acdcSelected(DeviceConstants::ACDC_t acdc);
private:
        Ui::ChannelControl *ui;
        DeviceConstants::Channel_t channel;
};

#endif // CHANNELCONTROL_H
