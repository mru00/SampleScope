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
