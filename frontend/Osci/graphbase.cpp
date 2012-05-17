#include "graphbase.h"
#include "ui_graphbase.h"

#include <qwt/qwt_plot_renderer.h>
#include <qwt/qwt_scale_widget.h>


GraphBase::GraphBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphBase)
  , plotGrid(new QwtPlotGrid())
  , zoomer(NULL)
{
    ui->setupUi(this);
    ui->qwtPlot->setCanvasBackground(QBrush(QColor(55, 143, 94)));

    zoomer = new QwtPlotZoomer(plot()->canvas());
    connect(zoomer, SIGNAL(zoomed(QRectF)), this, SLOT(zoomed(QRectF)));
    zoomer->setTrackerMode(QwtPlotZoomer::AlwaysOn);

    ui->qwtPlot->axisWidget(QwtPlot::yLeft)->setSpacing(500);
    plotGrid->attach(ui->qwtPlot);
    ui->qwtPlot->setAutoReplot(false);
}

GraphBase::~GraphBase()
{
    delete ui;
    delete zoomer;
    delete plotGrid;
}

void GraphBase::zoomed(const QRectF&) {
    // there is a bug in QwtPlotZoomer:
    // if the zoom stack is popped to the last element,
    // the last AxisScale is set. However, if the
    // plot was using AutoScale, the plot is messed up.
    if ( zoomer->zoomRectIndex() == 0 ) {
        setScale();
        zoomer->setZoomBase();
    }
}

QwtPlot* GraphBase::plot() const {
    return ui->qwtPlot;
}

void GraphBase::saveImage(QString filename) {

//    QwtPlotRenderer ren;
//    ren.renderDocument(ui->qwtPlot, filename, QSize(640,480));
}

QColor GraphBase::getChannelColor(DeviceConstants::Channel_t ch) {
    switch(ch) {
    case DeviceConstants::ADC_ch1:
        return QColor(0,255,148);
    case DeviceConstants::ADC_ch2:
        return Qt::blue;
    default:
        Q_ASSERT(ch == DeviceConstants::ADC_triggerLevel);
        return Qt::cyan;
    }
}
