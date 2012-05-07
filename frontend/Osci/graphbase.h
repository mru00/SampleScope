#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <QWidget>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>

#include "modecontrol.h"

namespace Ui {
    class GraphBase;
}

class GraphBase : public QWidget
{
    Q_OBJECT

public:
    explicit GraphBase(QWidget *parent = 0);
    virtual ~GraphBase();

    void saveImage(QString filename);
protected:
    Ui::GraphBase *ui;
    QwtPlot* plot();

protected:
    QwtPlotGrid *plotGrid;
};

#endif // GRAPHBASE_H
