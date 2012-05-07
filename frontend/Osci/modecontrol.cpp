#include "modecontrol.h"
#include "ui_modecontrol.h"

ModeControl::ModeControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeControl)
{
    ui->setupUi(this);

    connect(ui->listWidgetModes, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));
    connect(ui->radioButtonContinuous, SIGNAL(toggled(bool)), this, SLOT(continuousToggled(bool)));
    connect(ui->radioButtonSingleShot, SIGNAL(toggled(bool)), this, SLOT(singleToggled(bool)));
    connect(ui->pushButtonSample, SIGNAL(clicked()), this, SLOT(singleShotClicked()));

    connect(ui->listWidgetDummy, SIGNAL(itemSelectionChanged()), this, SLOT(dummySelectionChanged()));

    const char *modes[] = {
        "Ch 1",
        "Ch 2",
        "Interleaved",
        "Sequential"
    };

    for (int i = 0; i <= Mode_LAST; i++ ) {
        ui->listWidgetModes->insertItem(i, modes[i]);
    }


    const char* dummies[] = {
        "normal",
        "dummy tri",
        "dummy zero",
        "dummy mid"
    };

    for (int i = 0; i <= DeviceConstants::Dummy_LAST; i++ ) {
        ui->listWidgetDummy->insertItem(i, dummies[i]);
    }

    ui->listWidgetModes->setCurrentRow(0);
    ui->radioButtonContinuous->setChecked(true);
}

ModeControl::~ModeControl()
{
    delete ui;
}

void ModeControl::initialEmit() {

    itemSelectionChanged();

    bool single = ui->radioButtonSingleShot->isChecked();

    continuousToggled(!single);
    singleToggled(single);
}

void ModeControl::itemSelectionChanged() {
    int mode = ui->listWidgetModes->currentRow();
    emit modeSelected((ModeControl::Modes_t) mode);
}

void ModeControl::dummySelectionChanged() {
    int mode = ui->listWidgetDummy->currentRow();
    emit dummySelected((DeviceConstants::Dummy_t) mode);
}

void ModeControl::continuousToggled(bool val) {
    if (val) emit updateSelected(ModeControl::Update_Continuous);
}

void ModeControl::singleToggled(bool val) {
    if (val) emit updateSelected(ModeControl::Update_Single);
    ui->pushButtonSample->setEnabled(val);
}
void ModeControl::singleShotClicked()  {
    emit singleShot();
}
