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


#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <QWidget>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_magnifier.h>
#include "../../shared/common.h"

#include "modecontrol.h"

namespace Ui {
    class GraphBase;
}

/** base class for the plots */
class GraphBase : public QWidget
{
    Q_OBJECT

public:
    explicit GraphBase(QWidget *parent = 0);
    virtual ~GraphBase();

    void saveImage(QString filename);

    static QColor getChannelColor(DeviceConstants::Channel_t);
protected:
    QwtPlot* plot() const;
    virtual void setScale() = 0;
private slots:
    void zoomed(const QRectF&);
private:
    Ui::GraphBase *ui;
    QwtPlotGrid *plotGrid;
    QwtPlotZoomer *zoomer;
};

#endif // GRAPHBASE_H
