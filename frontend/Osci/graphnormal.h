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


#ifndef GRAPHNORMAL_H
#define GRAPHNORMAL_H


#include "graphbase.h"
#include "measurement.h"




class GraphNormal : public GraphBase
{
    Q_OBJECT

public:
    class Curve {
        public:
        QwtPlotCurve *data, *fittedSine, *rms, *avg;
        bool vis;
        DeviceConstants::Channel_t ch;
        Curve(DeviceConstants::Channel_t num)
            : data(new QwtPlotCurve())
            , fittedSine(new QwtPlotCurve())
            , rms(new QwtPlotCurve())
            , avg(new QwtPlotCurve())
            , vis(true)
            , ch(num)
        {
            data->setPen(QPen(QBrush(GraphBase::getChannelColor(ch)), 4));
            fittedSine->setPen(QPen(QBrush(GraphBase::getChannelColor(ch).darker()), 2));
        }
        virtual ~Curve() {
            delete data;
            delete fittedSine;
            delete rms;
            delete avg;
        }
        void attach(QwtPlot* plot) {
            data->attach(plot);
            fittedSine->attach(plot);
            rms->attach(plot);
            avg->attach(plot);
        }
        void setVisible(bool vis) {
            data->setVisible(vis);
            fittedSine->setVisible(vis);
            rms->setVisible(vis);
            avg->setVisible(vis);
            this->vis = vis;
        }
        bool isVisible() { return vis; }
        void setSamples(const Measurement::ChannelData& data);
    };

public:
    explicit GraphNormal(QWidget *parent = 0);
    virtual ~GraphNormal();

    void setData(const Measurement&, const QVector<QPointF>& tr);

protected:
    virtual void setScale();
signals:

public slots:
    void modeSelectionChanged(ModeControl::Modes_t);
private:

    Curve ch1, ch2;
    QwtPlotCurve
        *curveTriggerLevel;
};

#endif // GRAPHNORMAL_H
