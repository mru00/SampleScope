#ifndef GRAPHFFT_H
#define GRAPHFFT_H

#include "graphbase.h"
#include "measurement.h"


class GraphFFT : public GraphBase
{
    Q_OBJECT
public:
    explicit GraphFFT(QWidget *parent = 0);

    void setData(const Measurement&);

protected:
    virtual void setScale();
signals:

public slots:
    void modeSelectionChanged(ModeControl::Modes_t);

    private:

    QwtPlotCurve *curveFFTCh1, *curveFFTCh2;
};

#endif // GRAPHFFT_H
