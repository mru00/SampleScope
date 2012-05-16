#include "graphnormal.h"
#include "ui_graphbase.h"




GraphNormal::GraphNormal(QWidget *parent) :
    GraphBase(parent)
  , ch1(DeviceConstants::ADC_ch1)
  , ch2(DeviceConstants::ADC_ch2)
  , curveTriggerLevel(new QwtPlotCurve("Trigger Level"))
{

    ch1.attach(plot());
    ch2.attach(plot());
//    ch2.data->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
    curveTriggerLevel->attach(plot());
    curveTriggerLevel->setPen(QPen(QBrush(GraphBase::getChannelColor(DeviceConstants::ADC_triggerLevel)), 1));
    setScale();
}

GraphNormal::~GraphNormal() {
    delete curveTriggerLevel;
}

void GraphNormal::setScale() {
    plot()->setAxisScale(QwtPlot::xBottom, 0, 10);
    plot()->setAxisScale(QwtPlot::yLeft, -3, 3);
}


void GraphNormal::modeSelectionChanged(ModeControl::Modes_t mode) {
    ch1.setVisible(ModeControl::showsCh1(mode));
    ch2.setVisible(ModeControl::showsCh2(mode));
    if (mode == ModeControl::Mode_Interleaved) {
        //plot()->setAxisScale(QwtPlot::xBottom, 0, 5);
        plot()->setAxisAutoScale(QwtPlot::xBottom);
    }
    else {
        plot()->setAxisScale(QwtPlot::xBottom, 0, 10);
    }
    plot()->replot();
}

void GraphNormal::setData(const Measurement& meas, const QVector<QPointF>& dataTr) {

    if (!isVisible()) return;

    curveTriggerLevel->setSamples(dataTr);

    ch1.setSamples(meas.getChannel(DeviceConstants::ADC_ch1));
    ch2.setSamples(meas.getChannel(DeviceConstants::ADC_ch2));

    plot()->replot();

    saveImage("normal.png");
}

static inline QVector<QPointF> makeConstVector(const QVector<QPointF>& guide, double val) {
    QVector<QPointF> temp;
    temp.append(QPointF( guide.at(0).x(), val));
    temp.append( QPointF(guide.at(guide.size()-1).x(), val));
    return temp;
}

void GraphNormal::Curve::setSamples(const Measurement::ChannelData& measData) {

    if (!isVisible()) return;

    // this is an error condition and should be reported!
    if (measData.samples.size() == 0) return;

    data->setSamples(measData.samples);
    if (measData.fittedSine.size() > 0) {
        fittedSine->setSamples(measData.fittedSine);
        fittedSine->setVisible(true);
    }
    else {
        fittedSine->setVisible(false);
    }

    rms->setSamples(makeConstVector(measData.samples, measData.rms_div));
    avg->setSamples(makeConstVector(measData.samples, measData.avg_div));

}
