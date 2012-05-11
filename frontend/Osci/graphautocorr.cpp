#include "graphautocorr.h"

#include <qwt/qwt_symbol.h>

GraphAutoCorr::GraphAutoCorr(QWidget *parent) :
    GraphBase(parent)
  , curveCh1(new QwtPlotCurve("Ch1"))
  , curveCh2(new QwtPlotCurve("Ch2"))
  , markerMax1(new QwtPlotMarker())
{
    plot()->setAxisAutoScale(QwtPlot::yLeft, false);
    plot()->setAxisAutoScale(QwtPlot::yRight, false);
    plot()->setAxisScale(QwtPlot::yLeft, -3, 3);
    plot()->setAxisScale(QwtPlot::yRight, -3, 3);
    plot()->setCanvasBackground(QBrush(QColor(55, 143, 94)));

    curveCh1->attach(plot());
    curveCh1->setPen(QPen(QBrush(QColor(0,255,148)), 2));
    curveCh2->attach(plot());
    curveCh2->setPen(QPen(QBrush(Qt::blue), 2));

    QwtSymbol* sym = new QwtSymbol(QwtSymbol::Cross);
    sym->setSize(15);
    markerMax1->attach(plot());
    markerMax1->setSymbol(sym);

    plot()->setAxisAutoScale(QwtPlot::xBottom, true);
    plot()->setAxisAutoScale(QwtPlot::yLeft, true);
}
void GraphAutoCorr::modeSelectionChanged(ModeControl::Modes_t mode ) {
    curveCh1->setVisible(ModeControl::showsCh1(mode));
    curveCh2->setVisible(ModeControl::showsCh2(mode));
    plot()->replot();

}

void GraphAutoCorr::setData(const QVector<QPointF>& dataCh1, const QVector<QPointF>& dataCh2, int max_idx) {

    curveCh1->setSamples(dataCh1);
    curveCh2->setSamples(dataCh2);

    if (dataCh1.size() != 0) {
        markerMax1->setValue(dataCh1.at(max_idx).x(), dataCh1.at(max_idx).y());
        markerMax1->setVisible(true);
    }
    else {
        markerMax1->setVisible(false);
    }

    if (!isVisible()) return;

    plot()->replot();
    saveImage("fft.png");
}
