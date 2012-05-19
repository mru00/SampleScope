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


#include "graphcontrol.h"
#include "ui_graphcontrol.h"

GraphControl::GraphControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphControl)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

    const char *graphs[] = {
        "Normal",
        "XY",
        "FFT",
        "Autocorrelation",
        "Measurements"
    };

    for (int i = 0; i <= Graph_LAST; i++ ) {
        ui->listWidget->insertItem(i, graphs[i]);
    }

//    ui->listWidget->setCurrentRow(0);
//    ui->listWidget->item(Graph_AutoCorr)->setSelected(true);
    ui->listWidget->item(Graph_Normal)->setSelected(true);
    ui->listWidget->item(Graph_Measurements)->setSelected(true);
}

GraphControl::~GraphControl()
{
    delete ui;
}

void GraphControl::initialEmit() {
    itemSelectionChanged();
}

void GraphControl::itemSelectionChanged() {
    QMap<GraphControl::Graphs_t, bool> g;

    for (int i = 0; i <= Graph_LAST; i++ )
        g[(Graphs_t)i] = ui->listWidget->item(i)->isSelected();

    emit graphSelected(g);
}
