#include "calibdialog.h"
#include "ui_calibdialog.h"

#include "device.h"

CalibDialog::CalibDialog(Device* device,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibDialog),
    device(device)
{
    ui->setupUi(this);
    connect(ui->spinBoxDelay, SIGNAL(valueChanged(int)), this, SLOT(delayChanged(int)));
}

CalibDialog::~CalibDialog()
{
    delete ui;
}

void CalibDialog::delayChanged(int value) {
    device->setDelay(value);
}

void CalibDialog::setDelay(unsigned short value) {
    ui->spinBoxDelay->setValue(value);
}
