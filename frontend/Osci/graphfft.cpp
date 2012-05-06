#include "graphfft.h"
#include "ui_graphbase.h"

#include "qwt/qwt_plot_renderer.h"

#include <fftw3.h>

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

    if (!isVisible()) return;
#define NORM(x,y) sqrt( (x)*(x) + (y)*(y) )

    QVector<QPointF> dataFFT;
    int count = std::max(dataCh1.size(), dataCh2.size());
    double vals[count];
    fftw_complex out[count];


    if (dataCh1.size() > 0) {
    fftw_plan plan = fftw_plan_dft_r2c_1d(dataCh1.size(), vals, out, 0);
        for (int i = 0; i < dataCh1.size(); i ++ )
            vals[i] = dataCh1.at(i).y();
        fftw_execute(plan);

        for (int i = 0; i < dataCh1.size()/2; i ++ )
            dataFFT.append(QPointF(i, NORM(out[i][0], out[i][1])));
        curveFFTCh1->setSamples(dataFFT);

        dataFFT.clear();
    }


    if (dataCh2.size() > 0) {
    fftw_plan plan = fftw_plan_dft_r2c_1d(dataCh2.size(), vals, out, 0);
        for (int i = 0; i < dataCh2.size(); i ++ )
            vals[i] = dataCh2.at(i).y();
        fftw_execute(plan);
        for (int i = 0; i < dataCh2.size()/2; i ++ )
            dataFFT.append(QPointF(i, NORM(out[i][0], out[i][1])));
        curveFFTCh2->setSamples(dataFFT);
        fftw_destroy_plan(plan);
    }

    ui->qwtPlot->replot();
    saveImage("fft.png");
}
