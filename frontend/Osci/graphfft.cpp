#include "graphfft.h"
#include "ui_graphbase.h"

#include "qwt/qwt_plot_renderer.h"

GraphFFT::GraphFFT(QWidget *parent) :
    GraphBase(parent)
  , curveFFTCh1(new QwtPlotCurve("FFT Ch1"))
  , curveFFTCh2(new QwtPlotCurve("FFT Ch2"))
{
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft, false);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yRight, false);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -3, 3);
    ui->qwtPlot->setAxisScale(QwtPlot::yRight, -3, 3);
    ui->qwtPlot->setCanvasBackground(QBrush(QColor(55, 143, 94)));

    curveFFTCh1->attach(ui->qwtPlot);
    curveFFTCh1->setPen(QPen(QBrush(QColor(0,255,148)), 2));
    curveFFTCh2->attach(ui->qwtPlot);
    curveFFTCh2->setPen(QPen(QBrush(Qt::blue), 2));

    ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);
}

void GraphFFT::modeSelectionChanged(ModeControl::Modes_t mode ) {
    curveFFTCh1->setVisible(ModeControl::showsCh1(mode));
    curveFFTCh2->setVisible(ModeControl::showsCh2(mode));
    ui->qwtPlot->replot();

}

void GraphFFT::setData(const QVector<QPointF>& dataCh1, const QVector<QPointF>& dataCh2) {

    curveFFTCh1->setSamples(dataCh1);
    curveFFTCh2->setSamples(dataCh2);
    if (!isVisible()) return;

    ui->qwtPlot->replot();
    saveImage("fft.png");
}
