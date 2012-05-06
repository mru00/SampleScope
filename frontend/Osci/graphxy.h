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

public slots:
    void modeSelectionChanged(ModeControl::Modes_t);
private:

    QwtPlotCurve *curveXY;
};

#endif // GRAPHXY_H
