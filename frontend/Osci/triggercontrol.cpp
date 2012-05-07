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

    connect(ui->radioButtonRising, SIGNAL(toggled(bool)), this, SLOT(triggerRisingToggled(bool)));
    connect(ui->radioButtonFalling, SIGNAL(toggled(bool)), this, SLOT(triggerFallingToggled(bool)));
    ui->radioButtonFree->setChecked(true);
    ui->radioButtonRising->setChecked(true);
}

TriggerControl::~TriggerControl()
{
    delete ui;
}

void TriggerControl::initialEmit() {
    if (ui->radioButtonCh1->isChecked()) triggerCh1Toggled(true);
    else if (ui->radioButtonCh2->isChecked()) triggerCh2Toggled(true);
    else triggerFreeToggled(true);


    if (ui->radioButtonRising->isChecked()) triggerRisingToggled(true);
    else triggerFallingToggled(true);

}

void TriggerControl::triggerCh1Toggled(bool checked) {
    if (checked) emit triggerSourceSelected(DeviceConstants::TRIGGER_CH_1);
}
void TriggerControl::triggerCh2Toggled(bool checked) {
    if (checked) emit triggerSourceSelected(DeviceConstants::TRIGGER_CH_2);
}
void TriggerControl::triggerFreeToggled(bool checked) {
    if (checked) emit triggerSourceSelected(DeviceConstants::Trigger_Free);
}


void TriggerControl::triggerRisingToggled(bool val) {
    if(val) emit triggerModeSelected(DeviceConstants::TRIGGER_RISING);
}

void TriggerControl::triggerFallingToggled(bool val) {
    if(val) emit triggerModeSelected(DeviceConstants::TRIGGER_FALLING);
}
