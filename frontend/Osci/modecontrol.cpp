/*

SampleScope: A humble USB oscilloscope
Copyright (C) 2012 mru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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

    connect(ui->listWidgetDummy, SIGNAL(itemSelectionChanged()), this, SLOT(testSignalSelectionChanged()));

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
        "test tri",
        "test tri 2",
        "test zero",
        "test mid"
    };

    for (int i = 0; i <= DeviceConstants::TestSignal_LAST; i++ ) {
        ui->listWidgetDummy->insertItem(i, dummies[i]);
    }

    ui->listWidgetModes->setCurrentRow(0);
    ui->listWidgetDummy->setCurrentRow(0);
    ui->radioButtonContinuous->setChecked(true);
}

ModeControl::~ModeControl()
{
    delete ui;
}

void ModeControl::initialEmit() {

    itemSelectionChanged();
    testSignalSelectionChanged();

    bool single = ui->radioButtonSingleShot->isChecked();

    continuousToggled(!single);
    singleToggled(single);
}

void ModeControl::itemSelectionChanged() {
    int mode = ui->listWidgetModes->currentRow();
    emit modeSelected((ModeControl::Modes_t) mode);
}

void ModeControl::testSignalSelectionChanged() {
    int mode = ui->listWidgetDummy->currentRow();
    emit testSignalSelected((DeviceConstants::TestSignal_t) mode);
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
