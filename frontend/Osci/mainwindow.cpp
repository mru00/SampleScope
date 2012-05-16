#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"

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

    QMap<QAction*, QStyle::StandardPixmap> buttonIcons;
    QStyle* s = QApplication::style();
    buttonIcons[ui->actionConnect] = QStyle::SP_MediaPlay;
    buttonIcons[ui->actionDisconnect] = QStyle::SP_MediaStop;
    buttonIcons[ui->actionAuto_connect] = QStyle::SP_BrowserReload;
    buttonIcons[ui->actionUse_real_hardware] = QStyle::SP_DriveHDIcon;
    buttonIcons[ui->actionUse_software_emulation] = QStyle::SP_ComputerIcon;

    QMapIterator<QAction*, QStyle::StandardPixmap> i(buttonIcons);
    while (i.hasNext()) {
        i.next();
        i.key()->setIcon(s->standardIcon(i.value()));
    }

    connect(&updateTimer, SIGNAL(timeout()), &device, SLOT(refresh()));
    connect(&sampleTimer, SIGNAL(timeout()), this, SLOT(sample()));
    connect(&device, SIGNAL(connected(QString, QString)), this, SLOT(deviceConnected(QString, QString)));
    connect(&device, SIGNAL(disconnected()), this, SLOT(deviceDisconnected()));
    connect(&device, SIGNAL(fatal(QString)), this, SLOT(fatal(QString)));

    connect(ui->channelControlCh1, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), this, SLOT(VdivCh1Adjusted(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh1, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), &meas, SLOT(setVdiv_Ch1(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh1, SIGNAL(acdcSelected(DeviceConstants::ACDC_t)), this, SLOT(ACDCCh1Adjusted(DeviceConstants::ACDC_t)));

    connect(ui->channelControlCh2, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), this, SLOT(VdivCh2Adjusted(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh2, SIGNAL(vdivSelected(DeviceConstants::VdivValues_t)), &meas, SLOT(setVdiv_Ch2(DeviceConstants::VdivValues_t)));
    connect(ui->channelControlCh2, SIGNAL(acdcSelected(DeviceConstants::ACDC_t)), this, SLOT(ACDCCh2Adjusted(DeviceConstants::ACDC_t)));

    connect(ui->triggerControl, SIGNAL(triggerSourceSelected(DeviceConstants::TriggerSource_t)), this, SLOT(triggerSourceSelected(DeviceConstants::TriggerSource_t)));
    connect(ui->triggerControl, SIGNAL(triggerModeSelected(DeviceConstants::TriggerMode_t)), this, SLOT(triggerModeSelected(DeviceConstants::TriggerMode_t)));

    connect(ui->dialTriggerLevel, SIGNAL(valueChanged(int)), this, SLOT(TriggerLevelAdjusted(int)));
    connect(ui->timeControl, SIGNAL(TdivChanged(DeviceConstants::TdivValues_t)), this, SLOT(TdivAdjusted(DeviceConstants::TdivValues_t)));
    connect(ui->timeControl, SIGNAL(TdivChanged(DeviceConstants::TdivValues_t)), &meas, SLOT(setTdiv(DeviceConstants::TdivValues_t)));

    connect(ui->actionExport_Data, SIGNAL(triggered()), this, SLOT(exportData()));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), this, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphNorm, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphXY, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphFFT, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(modeSelected(ModeControl::Modes_t)), ui->graphAutoCorr, SLOT(modeSelectionChanged(ModeControl::Modes_t)));
    connect(ui->modeControl, SIGNAL(updateSelected(ModeControl::Update_t)), this, SLOT(updateSelectionChanged(ModeControl::Update_t)));
    connect(ui->modeControl, SIGNAL(singleShot()), this, SLOT(singleShot()));
    connect(ui->modeControl, SIGNAL(testSignalSelected(DeviceConstants::TestSignal_t)), this, SLOT(TestSignalSelectionChanged(DeviceConstants::TestSignal_t)));

    connect(ui->graphControl, SIGNAL(graphSelected(QMap<GraphControl::Graphs_t,bool>)), this, SLOT(graphSelectionChanged(QMap<GraphControl::Graphs_t,bool>)));
    connect(ui->actionCalibration_Dialog, SIGNAL(triggered()), &calibDlg, SLOT(show()));

    statusBar()->showMessage("Searching for device. Tip: try software emulation.");
    centralWidget()->setEnabled(false);
    updateTimer.start(2500);
    sampleTimer.setInterval(500);

//    QSqlDatabase
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *) {
    device.disConnect();
    calibDlg.close();
}

void MainWindow::TestSignalSelectionChanged(DeviceConstants::TestSignal_t dummy) {
    device.setTestSignal(dummy);
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

void MainWindow::deviceConnected(QString manu, QString prod) {
    statusBar()->showMessage(QString ("Connected to %1 (%2)").arg(prod).arg(manu));
    centralWidget()->setEnabled(true);

        ui->modeControl->initialEmit();
        ui->triggerControl->initialEmit();
        ui->channelControlCh1->initialEmit();
        ui->channelControlCh2->initialEmit();
        ui->timeControl->initialEmit();
        ui->graphControl->initialEmit();
        TriggerLevelAdjusted(ui->dialTriggerLevel->value());
}

void MainWindow::deviceDisconnected() {
    statusBar()->showMessage("Device not connected");
    centralWidget()->setEnabled(false);
}


void MainWindow::exportData() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Data as"), QString(), tr("Matlab Script (*.m);;Any File (*.*)"));
    cout << "selected file: " << filename.constData() << endl;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    meas.writeMatlab(file);
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

    QVector<QPointF> dataCh1, dataCh2, dataTr;

    device.getADCBlock(DeviceConstants::ADC_triggerLevel, dataTr);

    if (currentMode == ModeControl::Mode_Interleaved) {
        device.getADCInterleaved(dataCh1, dataCh2);
    }
    else {
        if (ModeControl::showsCh1(currentMode)) {
            device.getADCBlock(DeviceConstants::ADC_ch1, dataCh1);
        }

        if (ModeControl::showsCh2(currentMode)) {
            device.getADCBlock(DeviceConstants::ADC_ch2, dataCh2);
        }
    }

    meas.setChannelData(dataCh1, dataCh2);
    model.setData(meas);

    ui->graphFFT->setData(meas);
    ui->graphNorm->setData(meas, dataTr);
    ui->graphXY->setData(dataCh1, dataCh2);
    ui->graphAutoCorr->setData(meas);
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionUse_real_hardware_triggered()
{
    device.disConnect();
    device.selectHardwareImplementation(AbstractHardware::Impl_Real);
    device.connect();
}

void MainWindow::on_actionUse_software_emulation_triggered()
{
    device.disConnect();
    device.selectHardwareImplementation(AbstractHardware::Impl_Dummy);
    device.connect();
}

void MainWindow::on_actionDisconnect_triggered()
{
    ui->actionAuto_connect->setChecked(false);
    device.disConnect();
}


void MainWindow::on_actionConnect_triggered()
{
    device.connect();
}

void MainWindow::on_actionAuto_connect_toggled(bool arg1)
{
    if (arg1) updateTimer.start();
    else updateTimer.stop();
}
