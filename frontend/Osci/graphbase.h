#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <QWidget>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_magnifier.h>
#include "../../shared/common.h"

#include "modecontrol.h"

namespace Ui {
    class GraphBase;
}

/** base class for the plots */
class GraphBase : public QWidget
{
    Q_OBJECT

public:
    explicit GraphBase(QWidget *parent = 0);
    virtual ~GraphBase();

    void saveImage(QString filename);

    static QColor getChannelColor(DeviceConstants::Channel_t);
protected:
    QwtPlot* plot() const;
    virtual void setScale() = 0;
private slots:
    void zoomed(const QRectF&);
private:
    Ui::GraphBase *ui;
    QwtPlotGrid *plotGrid;
    QwtPlotZoomer *zoomer;
};

#endif // GRAPHBASE_H
