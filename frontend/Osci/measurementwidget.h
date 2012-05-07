#ifndef MEASUREMENTWIDGET_H
#define MEASUREMENTWIDGET_H

#include <QWidget>
#include "measurementtablemodel.h"

namespace Ui {
    class MeasurementWidget;
}

class MeasurementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementWidget(QWidget *parent = 0);
    ~MeasurementWidget();

    void setModel(MeasurementTableModel* model);
private:
    Ui::MeasurementWidget *ui;
};

#endif // MEASUREMENTWIDGET_H
