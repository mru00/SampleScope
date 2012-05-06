#include "graphbase.h"
#include "ui_graphbase.h"

#include <qwt/qwt_plot_renderer.h>


GraphBase::GraphBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphBase)
  , plotGrid(new QwtPlotGrid())
{
    ui->setupUi(this);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft, false);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yRight, false);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -3, 3);
    ui->qwtPlot->setAxisScale(QwtPlot::yRight, -3, 3);
    ui->qwtPlot->setCanvasBackground(QBrush(QColor(55, 143, 94)));
    plotGrid->attach(ui->qwtPlot);
}

GraphBase::~GraphBase()
{
    delete ui;
}

void GraphBase::saveImage(QString filename) {

//    QwtPlotRenderer ren;
//    ren.renderDocument(ui->qwtPlot, filename, QSize(640,480));
}
