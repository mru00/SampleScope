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


#include "graphautocorr.h"


GraphAutoCorr::GraphAutoCorr(QWidget *parent) :
    GraphBase(parent)
  , ch1(DeviceConstants::ADC_ch1)
  , ch2(DeviceConstants::ADC_ch2)
{
    plot()->setTitle(tr("Autocorrelation"));
    ch1.attach(plot());
    ch2.attach(plot());
    setScale();
}

void GraphAutoCorr::setScale() {
    plot()->setAxisAutoScale(QwtPlot::xBottom, true);
    plot()->setAxisAutoScale(QwtPlot::yLeft, true);
}

void GraphAutoCorr::modeSelectionChanged(ModeControl::Modes_t mode ) {
    ch1.setVisible(ModeControl::showsCh1(mode));
    ch2.setVisible(ModeControl::showsCh2(mode));
    plot()->replot();
}

void GraphAutoCorr::Curve::setData(const Measurement::ChannelData& data) {
    curve->setSamples(data.acorr);
    if (data.apeak_pos != -1) {
        markerMax->setValue(data.acorr.at(data.apeak_pos).x(), data.acorr.at(data.apeak_pos).y());
        markerMax->setVisible(true);
    }
    else {
        markerMax->setVisible(false);
    }
}

void GraphAutoCorr::setData(const Measurement& meas) {
    ch1.setData(meas.getChannel1());
    ch2.setData(meas.getChannel2());

    if (!isVisible()) return;

    plot()->replot();
    saveImage("fft.png");
}
