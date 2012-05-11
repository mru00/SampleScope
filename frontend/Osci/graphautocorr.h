#ifndef GRAPHAUTOCORR_H
#define GRAPHAUTOCORR_H

#include "graphbase.h"
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_marker.h>

class GraphAutoCorr : public GraphBase
{
    Q_OBJECT
public:
    explicit GraphAutoCorr(QWidget *parent = 0);

    void setData(const QVector<QPointF>& ch1, const QVector<QPointF>& ch2, int max_idx);
signals:

public slots:
    void modeSelectionChanged(ModeControl::Modes_t);
private:

    QwtPlotCurve *curveCh1, *curveCh2;
    QwtPlotMarker * markerMax1;
};

#endif // GRAPHAUTOCORR_H
