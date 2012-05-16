#ifndef GRAPHAUTOCORR_H
#define GRAPHAUTOCORR_H

#include "graphbase.h"
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_marker.h>

#include <qwt/qwt_symbol.h>
#include "measurement.h"

class GraphAutoCorr : public GraphBase
{
    Q_OBJECT

    class Curve {
       public:
        Curve(DeviceConstants::Channel_t ch)
            : curve(new QwtPlotCurve())
            , markerMax(new QwtPlotMarker())
            , ch(ch)
        {
            QwtSymbol* sym = new QwtSymbol(QwtSymbol::VLine);
            sym->setSize(24);
            sym->setPen(QPen( QBrush (GraphBase::getChannelColor(ch).lighter()) ,2 ));
            markerMax->setSymbol(sym);
            curve->setPen(QPen(QBrush(GraphBase::getChannelColor(ch)), 2));
        }
        QwtPlotCurve *curve;
        QwtPlotMarker * markerMax;
        DeviceConstants::Channel_t ch;
        void attach(QwtPlot* plot) {
            curve->attach(plot);
            markerMax->attach(plot);
        }
        void setData(const Measurement::ChannelData& data);
        void setVisible(bool vis) {
            curve->setVisible(vis);
            markerMax->setVisible(vis);
        }
    };
public:
    explicit GraphAutoCorr(QWidget *parent = 0);

    void setData(const Measurement&);
signals:

protected:
    virtual void setScale();
public slots:
    void modeSelectionChanged(ModeControl::Modes_t);
private:

    Curve ch1, ch2;
};

#endif // GRAPHAUTOCORR_H
