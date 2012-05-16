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
