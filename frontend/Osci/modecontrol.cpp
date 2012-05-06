#include "modecontrol.h"
#include "ui_modecontrol.h"

ModeControl::ModeControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeControl)
{
    ui->setupUi(this);

    connect(ui->listWidgetModes, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));


    const char *modes[] = {
        "Ch 1",
        "Ch 2",
        "Interleaved",
        "Sequential"
    };

    for (int i = 0; i <= Mode_LAST; i++ ) {
        ui->listWidgetModes->insertItem(i, modes[i]);
    }

    ui->listWidgetModes->setCurrentRow(0);
}

ModeControl::~ModeControl()
{
    delete ui;
}

void ModeControl::initialEmit() {

    itemSelectionChanged();
}

void ModeControl::itemSelectionChanged() {
    int mode = ui->listWidgetModes->currentRow();
    emit modeSelected((ModeControl::Modes_t) mode);
}
