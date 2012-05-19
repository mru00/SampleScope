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


#include "graphfft.h"
#include "ui_graphbase.h"

#include "qwt/qwt_plot_renderer.h"

GraphFFT::GraphFFT(QWidget *parent) :
    GraphBase(parent)
  , curveFFTCh1(new QwtPlotCurve("FFT Ch1"))
  , curveFFTCh2(new QwtPlotCurve("FFT Ch2"))
{
    plot()->setTitle(tr("FFT"));
    curveFFTCh1->attach(plot());
    curveFFTCh1->setPen(QPen(QBrush(GraphBase::getChannelColor(DeviceConstants::ADC_ch1)), 2));
    curveFFTCh2->attach(plot());
    curveFFTCh2->setPen(QPen(QBrush(GraphBase::getChannelColor(DeviceConstants::ADC_ch2)), 2));

    setScale();
}

void GraphFFT::setScale() {
    plot()->setAxisAutoScale(QwtPlot::xBottom, true);
    plot()->setAxisAutoScale(QwtPlot::yLeft, true);
}

void GraphFFT::modeSelectionChanged(ModeControl::Modes_t mode ) {
    curveFFTCh1->setVisible(ModeControl::showsCh1(mode));
    curveFFTCh2->setVisible(ModeControl::showsCh2(mode));
    plot()->replot();

}
void GraphFFT::setData(const Measurement& meas) {

    curveFFTCh1->setSamples(meas.getChannel1().fft);
    curveFFTCh2->setSamples(meas.getChannel2().fft);

    if (!isVisible()) return;

    plot()->replot();
    saveImage("fft.png");
}
