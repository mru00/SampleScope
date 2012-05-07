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
    , model(this)
    , currentMode(ModeControl::Mode_Ch1)
    , calibDlg(&device, this)
{
    ui->setupUi(this);


    ui->channelControlCh1->setChannel(DeviceConstants::ADC_ch1);
    ui->channelControlCh2->setChannel(DeviceConstants::ADC_ch2);
    ui->widgetMeasurements->setModel(&model);


    connect(&updateTimer, SIGNAL(timeout()), &device, SLOT(refresh()));
    connect(&sampleTimer, SIGNAL(timeout()), this, SLOT(sample()));
    connect(&device, SIGNAL(connected(bool)), this, SLOT(deviceConnected(bool)));
    connect(&device, SIGNAL(fatal(QString)), this, SLOT(fatal(QString)));

    connect(ui->channelControlCh1, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), this, SLOT(VdivCh1Adjusted(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh1, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), &model, SLOT(setVdiv_Ch1(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh1, SIGNAL(acdcSelected(DeviceConstants::ACDC_t)), this, SLOT(ACDCCh1Adjusted(DeviceConstants::ACDC_t)));

    connect(ui->channelControlCh2, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), this, SLOT(VdivCh2Adjusted(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh2, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), &model, SLOT(setVdiv_Ch2(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh2, SIGNAL(acdcSelected(DeviceConstants::ACDC_t)), this, SLOT(ACDCCh2Adjusted(DeviceConstants::ACDC_t)));

    connect(ui->triggerControl, SIGNAL(triggerSourceSelected(DeviceConstants::TriggerSource_t)), this, SLOT(triggerSourceSelected(DeviceConstants::TriggerSource_t)));
    connect(ui->triggerControl, SIGNAL(triggerModeSelected(DeviceConstants::TriggerMode_t)), this, SLOT(triggerModeSelected(DeviceConstants::TriggerMode_t)));

    connect(ui->dialTriggerLevel, SIGNAL(valueChanged(int)), this, SLOT(TriggerLevelAdjusted(int)));
    connect(ui->timeControl, SIGNAL(TdivChanged(DeviceConstants::TdivValues_t)), this, SLOT(TdivAdjusted(DeviceConstants::TdivValues_t)));
    connect(ui->timeControl, SIGNAL(TdivChanged(DeviceConstants::TdivValues_t)), &model, SLOT(setTdiv(DeviceConstants::TdivValues_t)));

    connect(ui->actionExport_Data, SIGNAL(triggered()), this, SLOT(exportData()));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), this, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphNorm, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphXY, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphFFT, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphAutoCorr, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(updateSelected(ModeControl::Update_t)), this, SLOT(updateSelectionChanged(ModeControl::Update_t)));
    connect(ui->modeControl, SIGNAL(singleShot()), this, SLOT(singleShot()));
    connect(ui->modeControl, SIGNAL(dummySelected(DeviceConstants::Dummy_t)), this, SLOT(dummySelectionChanged(DeviceConstants::Dummy_t)));

    connect(ui->graphControl, SIGNAL(graphSelected(QMap<GraphControl::Graphs_t,bool>)), this, SLOT(graphSelectionChanged(QMap<GraphControl::Graphs_t,bool>)));
    connect(ui->actionCalibration_Dialog, SIGNAL(triggered()), &calibDlg, SLOT(show()));

    statusBar()->showMessage("searching for device");
    centralWidget()->setEnabled(false);
    updateTimer.start(500);
    sampleTimer.setInterval(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *) {
    calibDlg.close();
}

void MainWindow::dummySelectionChanged(DeviceConstants::Dummy_t dummy) {
    device.setDummy(dummy);
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


void MainWindow::updateSelectionChanged(ModeControl::Update_t update) {
    switch (update) {
    case ModeControl::Update_Continuous:
        sampleTimer.start();
        break;
    case ModeControl::Update_Single:
        sampleTimer.stop();
        break;
    }
}

void MainWindow::singleShot() {
    sample();
}

void MainWindow::graphSelectionChanged(QMap<GraphControl::Graphs_t,bool> enabled) {

    ui->graphFFT->setVisible(enabled[GraphControl::Graph_FFT]);
    ui->graphNorm->setVisible(enabled[GraphControl::Graph_Normal]);
    ui->graphXY->setVisible(enabled[GraphControl::Graph_XY]);
    ui->graphAutoCorr->setVisible(enabled[GraphControl::Graph_AutoCorr]);
    ui->widgetMeasurements->setVisible(enabled[GraphControl::Graph_Measurements]);
}

void MainWindow::triggerSourceSelected(DeviceConstants::TriggerSource_t triggerSource) {
    device.selectTriggerSource(triggerSource);
    //curveTriggerLevel->setAxes(QwtPlot::xBottom, (triggerSource == Device::Trigger_Ch1) ? QwtPlot::yLeft : QwtPlot::yRight);
}

void MainWindow::triggerModeSelected(DeviceConstants::TriggerMode_t triggerMode) {
    device.selectTriggerMode(triggerMode);
}

void MainWindow::TdivAdjusted(DeviceConstants::TdivValues_t delay) {
    device.setTdiv(delay);
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

void MainWindow::ACDCCh1Adjusted(DeviceConstants::ACDC_t value) {
    device.setACDC_Ch1(value);
}

void MainWindow::ACDCCh2Adjusted(DeviceConstants::ACDC_t value) {
    device.setACDC_Ch2(value);
}

void MainWindow::VdivCh1Adjusted(DeviceConstants::VdivValues_t value) {
    device.setVdiv_Ch1(value);
}
void MainWindow::VdivCh2Adjusted(DeviceConstants::VdivValues_t value) {
    device.setVdiv_Ch2(value);
}

void MainWindow::TriggerLevelAdjusted(int value) {
    device.setTriggerLevel(value);
}

void MainWindow::sample() {

    if (!device.isConnected()) return;

    if (!ui->checkBoxActive->isChecked()) return;

    QVector<QPointF> dataCh1, dataCh2, dataTr;

    device.selectChannel(DeviceConstants::ADC_triggerLevel);
    device.getADCBlock(dataTr);

    if (currentMode == ModeControl::Mode_Interleaved) {
        device.getADCInterleaved(dataCh1, dataCh2);
    }
    else {
        if (ModeControl::showsCh1(currentMode)) {
            device.selectChannel(DeviceConstants::ADC_ch1);
            device.getADCBlock(dataCh1);
        }

        if (ModeControl::showsCh2(currentMode)) {
            device.selectChannel(DeviceConstants::ADC_ch2);
            device.getADCBlock(dataCh2);
        }
    }

    model.setChannelData(dataCh1, dataCh2);

    ui->graphFFT->setData(model.getFFT(DeviceConstants::ADC_ch1), model.getFFT(DeviceConstants::ADC_ch2));
    ui->graphNorm->setData(dataCh1, dataCh2, dataTr);
    ui->graphXY->setData(dataCh1, dataCh2);
    ui->graphAutoCorr->setData(model.getAutoCorr(DeviceConstants::ADC_ch1), model.getAutoCorr(DeviceConstants::ADC_ch2));
}
