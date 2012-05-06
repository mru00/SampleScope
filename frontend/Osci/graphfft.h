#ifndef GRAPHFFT_H
#define GRAPHFFT_H

#include "graphbase.h"


class GraphFFT : public GraphBase
{
    Q_OBJECT
public:
    explicit GraphFFT(QWidget *parent = 0);

    void setData(const QVector<QPointF>& ch1, const QVector<QPointF>& ch2);
signals:

public slots:
    void modeSelectionChanged(ModeControl::Modes_t);

    private:

    QwtPlotCurve *curveFFTCh1, *curveFFTCh2;
};

#endif // GRAPHFFT_H
