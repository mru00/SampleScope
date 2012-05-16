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
