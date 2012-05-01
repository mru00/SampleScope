#include "channelcontrol.h"
#include "ui_channelcontrol.h"

ChannelControl::ChannelControl(Device::Channel_t id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelControl),
        channel(id)
{
    ui->setupUi(this);
    const char* channelNames[] = {
            "Channel 1",
            "Channel 2",
            "Trigger"
    };
    ui->groupBox->setTitle(channelNames[id]);

    connect(ui->dialGain, SIGNAL(valueChanged(int)), this, SLOT(vdivValueChanged(int)));
    connect(ui->radioButtonAC, SIGNAL(toggled(bool)), this, SLOT(acdcValueChanged(bool)));
    ui->radioButtonAC->setChecked(false);
    ui->dialGain->setRange(0, Device::Vdiv_LAST);
}

ChannelControl::~ChannelControl()
{
    delete ui;
}

void ChannelControl::initialEmit() {
        acdcValueChanged(ui->radioButtonAC->isChecked());
        vdivValueChanged(ui->dialGain->value());
}

void ChannelControl::acdcValueChanged(bool acdc) {
        emit acdcSelected(acdc ? Device::ACDC_AC : Device::ACDC_DC);
}

void ChannelControl::vdivValueChanged(int value_) {

        Device::VdivValues_t value = (Device::VdivValues_t)value_;
        QString label = Device::getVdivLabel(value);

        ui->labelGain->setText(label);
        emit vdivSelected(label, value);
}

bool ChannelControl::isActive() {
        return ui->checkBoxActive->isChecked();
}

bool ChannelControl::fitSin() {
        return ui->checkBoxFitSin->isChecked();
}

void ChannelControl::setFitData(double a0, double a1, double a2) {
        ui->labelFit->setText(QString("%1 * sin (%2 + %3*t)").arg(a0).arg(a1).arg(a2));
}
