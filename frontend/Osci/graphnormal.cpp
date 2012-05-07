#include "graphnormal.h"
#include "ui_graphbase.h"

#define BUG 5
#include "lmmin.h"
#include "lm_eval.h"



GraphNormal::GraphNormal(QWidget *parent) :
    GraphBase(parent)
  , ch1(1)
  , ch2(2)
  , curveTriggerLevel(new QwtPlotCurve("Trigger Level"))
{

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 10);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -3, 3);
    ui->qwtPlot->setCanvasBackground(QBrush(QColor(55, 143, 94)));
    ch1.attach(ui->qwtPlot);
    ch2.attach(ui->qwtPlot);
    ch1.data->setPen(QPen(QBrush(QColor(0,255,148)), 4));
    ch2.data->setPen(QPen(QBrush(Qt::blue), 3));
    ch2.data->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
    curveTriggerLevel->attach(ui->qwtPlot);
    curveTriggerLevel->setPen(QPen(QBrush(Qt::yellow), 1));

    ch1.fittedSine->setPen(QPen(QBrush(Qt::darkGreen), 2));
}

GraphNormal::~GraphNormal() {
    delete curveTriggerLevel;
}

struct fit_data_t {
    double* t;
    double* y;
};

static double sin_par(double t, double p[3]) {
    return p[0] * sin ( p[1] + p[2]*t );
}

static void lm_evaluate(double par[3], int m_dat, double fvec1[], void* obj, int *)
{
    fit_data_t* that = (fit_data_t*) obj;
    for (int i = 0; i < m_dat; i++) {
        fvec1[i] = that->y[i] - sin_par(that->t[i], par);
    }
}

bool fitSin(const QVector<QPointF>& samples, double p[], QVector<QPointF>& result) {

    double fit_t[samples.size()];
    double fit_y[samples.size()];

    for (int i = 0; i< samples.size(); i ++ ) {
        fit_t[i] = samples.at(i).x();
        fit_y[i] = samples.at(i).y();
    }

    // auxiliary settings:
    lm_control_type control;
    lm_initialize_control(&control);

    fit_data_t fit_data;
    fit_data.t = fit_t;
    fit_data.y = fit_y;

    control.maxcall = 3000;
    //control.ftol = 1e-20;
    //control.xtol = 1e-20;

    lm_minimize(samples.size(), 3, p,
                lm_evaluate, lm_print_default,
                &fit_data, &control);

    if (control.info != 1 && control.info != 2 && control.info != 3) {

        p[0] = 1.0;
        p[1] = 0.0;
        p[2] = 200;

        return false;
    }


    result.clear();
    for (int i = 0; i<samples.size(); i ++ ) {
        result.append(QPointF(fit_t[i], sin_par(fit_t[i], p) ));
    }
    return true;
}

static double rms(const QVector<QPointF>& data) {
    double sum = 0;
    for (int i = 0; i < data.size(); i++ ) {
        double t = data.at(i).y();
        sum += t*t;
    }
    sum /= data.size();
    return sqrt(sum);
}

double avg(const QVector<QPointF>& data) {
    double sum = 0;
    for (int i = 0; i < data.size(); i++ ) {
        double t = data.at(i).y();
        sum += t;
    }
    sum /= data.size();
    return sum;
}


void GraphNormal::modeSelectionChanged(ModeControl::Modes_t mode) {
    ch1.setVisible(ModeControl::showsCh1(mode));
    ch2.setVisible(ModeControl::showsCh2(mode));
    if (mode == ModeControl::Mode_Interleaved) {
        //ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 5);
        ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
    }
    else {
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 10);
    }
    ui->qwtPlot->replot();
}

void GraphNormal::setData(const QVector<QPointF>& dataCh1, const QVector<QPointF>& dataCh2, const QVector<QPointF>& dataTr) {

    if (!isVisible()) return;

    curveTriggerLevel->setSamples(dataTr);

    ch1.setSamples(dataCh1);
    ch2.setSamples(dataCh2);

    ui->qwtPlot->replot();

    saveImage("normal.png");
}

static inline QVector<QPointF> makeConstVector(const QVector<QPointF>& guide, double val) {
    QVector<QPointF> temp;
    temp.append(QPointF( guide.at(0).x(), val));
    temp.append( QPointF(guide.at(guide.size()-1).x(), val));
    return temp;
}

void GraphNormal::Curve::setSamples(const QVector<QPointF>& samples) {

    if (!isVisible()) return;

    // this is an error condition and should be reported!
    if (samples.size() == 0) return;

    QVector<QPointF> dataFit;

    data->setSamples(samples);
    if (fitSin(samples, p, dataFit)) {
        fittedSine->setSamples(dataFit);
        fittedSine->setVisible(true);
    }
    else {
        fittedSine->setVisible(false);
    }

    rms->setSamples(makeConstVector(samples, ::rms(samples)));
    avg->setSamples(makeConstVector(samples, ::avg(samples)));

}
