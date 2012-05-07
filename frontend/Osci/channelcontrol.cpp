#include "channelcontrol.h"
#include "ui_channelcontrol.h"

ChannelControl::ChannelControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelControl)
{
    ui->setupUi(this);

    connect(ui->dialGain, SIGNAL(valueChanged(int)), this, SLOT(vdivValueChanged(int)));
    connect(ui->radioButtonAC, SIGNAL(toggled(bool)), this, SLOT(acdcValueChanged(bool)));
    ui->radioButtonAC->setChecked(false);
    ui->dialGain->setRange(0, DeviceConstants::Vdiv_LAST);
}

ChannelControl::~ChannelControl()
{
    delete ui;
}

void ChannelControl::setChannel(DeviceConstants::Channel_t c) {
        channel = c;
        ui->groupBox->setTitle(Device::getChannelName(channel));
}

void ChannelControl::initialEmit() {
        acdcValueChanged(ui->radioButtonAC->isChecked());
        vdivValueChanged(ui->dialGain->value());
}

void ChannelControl::acdcValueChanged(bool acdc) {
        emit acdcSelected(acdc ? DeviceConstants::ACDC_AC : DeviceConstants::ACDC_DC);
}

void ChannelControl::vdivValueChanged(int value_) {

        DeviceConstants::VdivValues_t value = (DeviceConstants::VdivValues_t)value_;
        QString label = Device::getVdivLabel(value);

        ui->labelGain->setText(label);
        emit vdivSelected(value);
}


bool ChannelControl::fitSin() {
        return ui->checkBoxFitSin->isChecked();
}

void ChannelControl::setFitData(double a0, double a1, double a2) {
        //ui->labelFit->setText(QString("%1 * sin (%2 + %3*t)").arg(a0).arg(a1).arg(a2));
}
