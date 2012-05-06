#ifndef GRAPHNORMAL_H
#define GRAPHNORMAL_H


#include "graphbase.h"




class GraphNormal : public GraphBase
{
    Q_OBJECT

public:
    class Curve {
        public:
        double p[3];
        QwtPlotCurve *data, *fittedSine, *rms, *avg;
        bool vis;
        Curve(int num)
            : data(new QwtPlotCurve(QString("Ch %1").arg(num)))
            , fittedSine(new QwtPlotCurve(QString("Sin %1").arg(num)))
            , rms(new QwtPlotCurve(QString("RMS %1").arg(num)))
            , avg(new QwtPlotCurve(QString("AVG %1").arg(num)))
            , vis(true)
        {
            p[0] = p[1] = p[2] = 1000.0;
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
        void setSamples(const QVector<QPointF>& data);
    };

public:
    explicit GraphNormal(QWidget *parent = 0);
    virtual ~GraphNormal();

    void setData(const QVector<QPointF>& ch1, const QVector<QPointF>& ch2, const QVector<QPointF>& tr);

signals:

public slots:
    void modeSelectionChanged(ModeControl::Modes_t);
private:

    Curve ch1, ch2;
    QwtPlotCurve
        *curveTriggerLevel;
};

#endif // GRAPHNORMAL_H
