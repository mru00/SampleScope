#include "measurementwidget.h"
#include "ui_measurementwidget.h"

MeasurementWidget::MeasurementWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasurementWidget)
{
    ui->setupUi(this);
}

MeasurementWidget::~MeasurementWidget()
{
    delete ui;
}

void MeasurementWidget::setModel(MeasurementTableModel* model) {
    ui->tableView->setModel(model);
}
