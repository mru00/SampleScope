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
