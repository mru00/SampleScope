#include <iostream>
#include <QFileDialog>
#include <QMessageBox>


#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , device()
    , vdiv_ch1(Device::Vdiv_10), vdiv_ch2(Device::Vdiv_10)
    , delay(Device::Tdiv_01)
    , currentMode(ModeControl::Mode_Ch1)
{
    ui->setupUi(this);


    ui->channelControlCh1->setChannel(Device::ADC_ch1);
    ui->channelControlCh2->setChannel(Device::ADC_ch2);


    connect(&updateTimer, SIGNAL(timeout()), &device, SLOT(refresh()));
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(sample()));
    connect(&device, SIGNAL(connected(bool)), this, SLOT(deviceConnected(bool)));
    connect(&device, SIGNAL(fatal(QString)), this, SLOT(fatal(QString)));

    connect(ui->channelControlCh1, SIGNAL(vdivSelected(QString, Device::VdivValues_t)), this, SLOT(VdivCh1Adjusted(QString, Device::VdivValues_t)));
    connect(ui->channelControlCh1, SIGNAL(acdcSelected(Device::ACDC_t)), this, SLOT(ACDCCh1Adjusted(Device::ACDC_t)));

    connect(ui->channelControlCh2, SIGNAL(vdivSelected(QString, Device::VdivValues_t)), this, SLOT(VdivCh2Adjusted(QString, Device::VdivValues_t)));
    connect(ui->channelControlCh2, SIGNAL(acdcSelected(Device::ACDC_t)), this, SLOT(ACDCCh2Adjusted(Device::ACDC_t)));

    connect(ui->triggerControl, SIGNAL(triggerSourceSelected(Device::TriggerSource_t)), this, SLOT(triggerSourceSelected(Device::TriggerSource_t)));

    connect(ui->dialTriggerLevel, SIGNAL(valueChanged(int)), this, SLOT(TriggerLevelAdjusted(int)));
    connect(ui->timeControl, SIGNAL(delayValueChanged(QString, Device::TdivValues_t)), this, SLOT(delayAdjusted(QString, Device::TdivValues_t)));

    connect(ui->actionExport_Data, SIGNAL(triggered()), this, SLOT(exportData()));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), this, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphNorm, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphXY, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphFFT, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->graphControl, SIGNAL(graphSelected(QMap<GraphControl::Graphs_t,bool>)), this, SLOT(graphSelectionChanged(QMap<GraphControl::Graphs_t,bool>)));

    statusBar()->showMessage("searching for device");
    centralWidget()->setEnabled(false);
    updateTimer.start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::modeSelectionChanged(ModeControl::Modes_t mode) {
    currentMode = mode;

    switch(mode) {
    case ModeControl::Mode_Ch1:
        ui->channelControlCh1->setVisible(true);
        ui->channelControlCh2->setVisible(false);
        break;
    case ModeControl::Mode_Ch2:
        ui->channelControlCh1->setVisible(false);
        ui->channelControlCh2->setVisible(true);
        break;
    case ModeControl::Mode_Interleaved:
        ui->channelControlCh1->setVisible(true);
        ui->channelControlCh2->setVisible(true);
        break;
    case ModeControl::Mode_Sequential:
        ui->channelControlCh1->setVisible(true);
        ui->channelControlCh2->setVisible(true);
        break;
    }
}

void MainWindow::graphSelectionChanged(QMap<GraphControl::Graphs_t,bool> enabled) {

    ui->graphFFT->setVisible(enabled[GraphControl::Graph_FFT]);
    ui->graphNorm->setVisible(enabled[GraphControl::Graph_Normal]);
    ui->graphXY->setVisible(enabled[GraphControl::Graph_XY]);
}

void MainWindow::triggerSourceSelected(Device::TriggerSource_t triggerSource) {
    device.selectTriggerSource(triggerSource);
    //curveTriggerLevel->setAxes(QwtPlot::xBottom, (triggerSource == Device::Trigger_Ch1) ? QwtPlot::yLeft : QwtPlot::yRight);
}

void MainWindow::delayAdjusted(QString, Device::TdivValues_t delay) {
    this->delay = delay;
}

void MainWindow::deviceConnected(bool connected) {
    statusBar()->showMessage(connected ? "Connected" : "Device not connected");
    centralWidget()->setEnabled(connected);

    if (connected) {
        ui->modeControl->initialEmit();
        ui->triggerControl->initialEmit();
        ui->channelControlCh1->initialEmit();
        ui->channelControlCh2->initialEmit();
        ui->timeControl->initialEmit();
        ui->graphControl->initialEmit();
        TriggerLevelAdjusted(ui->dialTriggerLevel->value());
    }
}

void MainWindow::exportData() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Data as", QString(), "Data files (*.dat)");
    cout << "selected file: " << filename.constData() << endl;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    /*
        for ( int i = 0; i < 191; i ++ ) {
                out << i
                    << " " << curveTriggerLevel->sample(i).y()
                    << " " << curveCh1->sample(i).y()
                    << " " << curveCh2->sample(i).y()
                    << "\n";

}
*/
    file.close();
}

void MainWindow::fatal(QString message) {
    QMessageBox::critical(this, "Fatal", message);
    device.disConnect();
}

void MainWindow::ACDCCh1Adjusted(Device::ACDC_t value) {
    acdc_ch1 = value;
    device.setACDC(acdc_ch1, acdc_ch2);
}

void MainWindow::ACDCCh2Adjusted(Device::ACDC_t value) {
    acdc_ch2 = value;
    device.setACDC(acdc_ch1, acdc_ch2);
}

void MainWindow::VdivCh1Adjusted(QString, Device::VdivValues_t value) {
    vdiv_ch1 = value;
    device.setVdiv(vdiv_ch1, vdiv_ch2);
}
void MainWindow::VdivCh2Adjusted(QString, Device::VdivValues_t value) {
    vdiv_ch2 = value;
    device.setVdiv(vdiv_ch1, vdiv_ch2);
}

void MainWindow::TriggerLevelAdjusted(int value) {
    device.setTriggerLevel(value);
}

void MainWindow::sample() {

    if (!device.isConnected()) return;

    if (!ui->checkBoxActive->isChecked()) return;

    QVector<QPointF> dataCh1, dataCh2, dataTr;

    device.selectChannel(Device::ADC_triggerLevel);
    dataTr = device.getADCBlock();

    if (ModeControl::showsCh1(currentMode)) {
        device.selectChannel(Device::ADC_ch1);
        dataCh1 = device.getADCBlock(delay);
    }

    if (ModeControl::showsCh2(currentMode)) {
        device.selectChannel(Device::ADC_ch2);
        dataCh2 = device.getADCBlock(delay);
    }


    ui->graphNorm->setData(dataCh1, dataCh2, dataTr);
    ui->graphFFT->setData(dataCh1, dataCh2);
    ui->graphXY->setData(dataCh1, dataCh2);
}
