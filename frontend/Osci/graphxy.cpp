#include "graphxy.h"
#include "ui_graphbase.h"

GraphXY::GraphXY(QWidget *parent) :
    GraphBase(parent)
  , curveXY(new QwtPlotCurve("XY"))
{
    plot()->setTitle("XY");
    curveXY->setPen(QPen(QBrush(Qt::yellow), 2));
    curveXY->attach(plot());
    setScale();
}


void GraphXY::setScale() {
    plot()->setAxisScale(QwtPlot::xBottom, -3, 3);
    plot()->setAxisScale(QwtPlot::yLeft, -3, 3);
}

void GraphXY::modeSelectionChanged(ModeControl::Modes_t mode) {
    switch(mode) {
    case ModeControl::Mode_Ch1:
    case ModeControl::Mode_Ch2:
        curveXY->setVisible(false);
        break;
    case ModeControl::Mode_Interleaved:
    case ModeControl::Mode_Sequential:
        curveXY->setVisible(true);
        break;
    }
    plot()->replot();
}
void GraphXY::setData(const QVector<QPointF>& dataCh1, const QVector<QPointF>& dataCh2) {

    if (!isVisible()) return;
    if (!curveXY->isVisible()) return;

    QVector<QPointF> dataXY;
    for (int i = 0; i < dataCh1.size(); i ++ )
        dataXY.append(QPointF(dataCh1.at(i).y(), dataCh2.at(i).y()));
    curveXY->setSamples(dataXY);
    plot()->replot();
    saveImage("xy.png");
}
