#include "triggercontrol.h"
#include "ui_triggercontrol.h"

TriggerControl::TriggerControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerControl)
{
    ui->setupUi(this);
    connect(ui->radioButtonCh1, SIGNAL(toggled(bool)), this, SLOT(triggerCh1Toggled(bool)));
    connect(ui->radioButtonCh2, SIGNAL(toggled(bool)), this, SLOT(triggerCh2Toggled(bool)));
    connect(ui->radioButtonFree, SIGNAL(toggled(bool)), this, SLOT(triggerFreeToggled(bool)));
    ui->radioButtonFree->setChecked(true);
}

TriggerControl::~TriggerControl()
{
    delete ui;
}

void TriggerControl::initialEmit() {
        if (ui->radioButtonCh1->isChecked()) triggerCh1Toggled(true);
        else if (ui->radioButtonCh2->isChecked()) triggerCh2Toggled(true);
        triggerFreeToggled(true);
}

void TriggerControl::triggerCh1Toggled(bool checked) {
        if (checked) emit triggerSourceSelected(Device::Trigger_Ch1);
}
void TriggerControl::triggerCh2Toggled(bool checked) {
        if (checked) emit triggerSourceSelected(Device::Trigger_Ch2);
}
void TriggerControl::triggerFreeToggled(bool checked) {
        if (checked) emit triggerSourceSelected(Device::Trigger_Free);
}
