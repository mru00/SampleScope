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


#ifndef GRAPHXY_H
#define GRAPHXY_H

#include "graphbase.h"

class GraphXY : public GraphBase
{
    Q_OBJECT
public:
    explicit GraphXY(QWidget *parent = 0);

    void setData(const QVector<QPointF>& ch1, const QVector<QPointF>& ch2);
signals:

protected:
    virtual void setScale();
public slots:
    void modeSelectionChanged(ModeControl::Modes_t);
private:

    QwtPlotCurve *curveXY;
};

#endif // GRAPHXY_H
