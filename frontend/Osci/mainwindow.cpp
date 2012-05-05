#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

#include <fftw3.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#define BUG 5
#include "lmmin.h"
#include "lm_eval.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , device()
        , channelControl1(Device::ADC_ch1, this)
        , channelControl2(Device::ADC_ch2, this)
        , triggerControl(this)
        , timeControl(this)
        , curveCh1(new QwtPlotCurve("Ch 1"))
        , curveCh2(new QwtPlotCurve("Ch 2"))
        , curveTriggerLevel(new QwtPlotCurve("Trigger Level"))
        , curveFitSinCh1(new QwtPlotCurve("Fit Ch1"))
        , plotGrid(new QwtPlotGrid())
        , vdiv_ch1(Device::Vdiv_10), vdiv_ch2(Device::Vdiv_10)
        , delay(Device::Tdiv_01)
{
        p_ch1[0] = 1000;
        p_ch1[1] = 17;
        p_ch1[2] = 0.12;
        ui->setupUi(this);

        ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft, false);
        ui->qwtPlot->setAxisAutoScale(QwtPlot::yRight, false);
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -3, 3);
        ui->qwtPlot->setAxisScale(QwtPlot::yRight, -3, 3);
        ui->qwtPlot->setCanvasBackground(QBrush(QColor(55, 143, 94)));
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "Ch1 [div]");
        ui->qwtPlot->setAxisTitle(QwtPlot::yRight, "Ch2 [div]");
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "T [samples]");
        ui->qwtPlot->enableAxis(QwtPlot::yRight, true);
        curveCh1->attach(ui->qwtPlot);
        curveCh2->attach(ui->qwtPlot);
        curveCh1->setPen(QPen(QBrush(QColor(0,255,148)), 3));
        curveCh2->setPen(QPen(QBrush(Qt::blue), 3));
        curveCh2->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
        curveTriggerLevel->attach(ui->qwtPlot);
        curveTriggerLevel->setPen(QPen(QBrush(Qt::yellow), 1));
        curveFitSinCh1->attach(ui->qwtPlot);
        plotGrid->attach(ui->qwtPlot);

        ui->verticalLayoutChannelControl->addWidget(&channelControl1);
        ui->verticalLayoutChannelControl->addWidget(&channelControl2);
        ui->verticalLayoutChannelControl->addWidget(&triggerControl);
        ui->verticalLayoutChannelControl->addWidget(&timeControl);

        connect(&updateTimer, SIGNAL(timeout()), &device, SLOT(refresh()));
        connect(&updateTimer, SIGNAL(timeout()), this, SLOT(sample()));
        connect(&device, SIGNAL(connected(bool)), this, SLOT(deviceConnected(bool)));
        connect(&device, SIGNAL(fatal(QString)), this, SLOT(fatal(QString)));

        connect(&channelControl1, SIGNAL(vdivSelected(QString, Device::VdivValues_t)), this, SLOT(VdivCh1Adjusted(QString, Device::VdivValues_t)));
        connect(&channelControl1, SIGNAL(acdcSelected(Device::ACDC_t)), this, SLOT(ACDCCh1Adjusted(Device::ACDC_t)));

        connect(&channelControl2, SIGNAL(vdivSelected(QString, Device::VdivValues_t)), this, SLOT(VdivCh2Adjusted(QString, Device::VdivValues_t)));
        connect(&channelControl2, SIGNAL(acdcSelected(Device::ACDC_t)), this, SLOT(ACDCCh2Adjusted(Device::ACDC_t)));

        connect(&triggerControl, SIGNAL(triggerSourceSelected(Device::TriggerSource_t)), this, SLOT(triggerSourceSelected(Device::TriggerSource_t)));

        connect(ui->dialTriggerLevel, SIGNAL(valueChanged(int)), this, SLOT(TriggerLevelAdjusted(int)));
        connect(&timeControl, SIGNAL(delayValueChanged(QString, Device::TdivValues_t)), this, SLOT(delayAdjusted(QString, Device::TdivValues_t)));

        connect(ui->actionExport_Data, SIGNAL(triggered()), this, SLOT(exportData()));

        statusBar()->showMessage("searching for device");
        centralWidget()->setEnabled(false);
        updateTimer.start(500);
}

MainWindow::~MainWindow()
{
        delete ui;
}
void MainWindow::triggerSourceSelected(Device::TriggerSource_t triggerSource) {
        device.selectTriggerSource(triggerSource);
        curveTriggerLevel->setAxes(QwtPlot::xBottom, (triggerSource == Device::Trigger_Ch1) ? QwtPlot::yLeft : QwtPlot::yRight);
}

void MainWindow::delayAdjusted(QString, Device::TdivValues_t delay) {
        this->delay = delay;
}

void MainWindow::deviceConnected(bool connected) {
        statusBar()->showMessage(connected ? "Connected" : "Device not connected");
        centralWidget()->setEnabled(connected);

        if (connected) {
             //   triggerControl.initialEmit();
                channelControl1.initialEmit();
                channelControl2.initialEmit();
                timeControl.initialEmit();
                TriggerLevelAdjusted(ui->dialTriggerLevel->value());
        }
}

void MainWindow::exportData() {
        QString filename = QFileDialog::getSaveFileName(this, "Save Data as", QString(), "Data files (*.dat)");
        cout << "selected file: " << filename.constData() << endl;
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);

        for ( int i = 0; i < 191; i ++ ) {
                out << i
                    << " " << curveTriggerLevel->sample(i).y()
                    << " " << curveCh1->sample(i).y()
                    << " " << curveCh2->sample(i).y()
                    << "\n";
        }
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

struct fit_data_t {
        double* t;
        double* y;
};

static double fit_sin(double t, double p[3]) {
        return p[0] * sin ( p[1] + p[2]*t );
}

static void lm_evaluate(double par[3], int m_dat, double fvec1[], void* obj, int *)
{
        fit_data_t* that = (fit_data_t*) obj;
        for (int i = 0; i < m_dat; i++) {
                fvec1[i] = that->y[i] - fit_sin(that->t[i], par);
        }
}

void MainWindow::sample() {

        return;
        if (!device.isConnected()) return;


        // read trigger level

        device.selectChannel(Device::ADC_triggerLevel);
        curveTriggerLevel->setSamples(device.getADCBlock());


        if (ui->checkBoxActive->isChecked()) {
                if (channelControl1.isActive()) {

                        curveCh1->setVisible(true);
                        device.selectChannel(Device::ADC_ch1);
                        QVector<QPointF> data = device.getADCBlock(delay);
                        curveCh1->setSamples(data);

                        if (channelControl1.fitSin()) {

                                double fit_t[data.size()];
                                double fit_y[data.size()];

                                for (int i = 0; i< data.size(); i ++ ) {
                                        fit_t[i] = data.at(i).x();
                                        fit_y[i] = data.at(i).y();
                                }

                                // auxiliary settings:
                                lm_control_type control;
                                lm_initialize_control(&control);

                                fit_data_t fit_data;
                                fit_data.t = fit_t;
                                fit_data.y = fit_y;

                                control.maxcall = 3000;
                                //control.ftol = 1e-20;
                                //control.xtol = 1e-20;

                                lm_minimize(data.size(), 3, p_ch1,
                                            lm_evaluate, lm_print_default,
                                            &fit_data, &control);

                                if (control.info == 1 || control.info == 2 || control.info == 3) {
                                        QVector<QPointF> dataFit;
                                        for (int i = 0; i<data.size(); i ++ ) {
                                                dataFit.append(QPointF(fit_t[i], fit_sin(fit_t[i], p_ch1) ));
                                        }
                                        curveFitSinCh1->setSamples(dataFit);
                                        curveFitSinCh1->setVisible(true);
                                }
                                else {

                                        p_ch1[0] = 1.0;
                                        p_ch1[1] = 0.0;
                                        p_ch1[2] = 200;
                                        curveFitSinCh1->setVisible(false);
                                }
                        }
                }
                else {
                        curveCh1->setVisible(false);
                }

                if (channelControl2.isActive()) {
                        curveCh2->setVisible(true);
                        device.selectChannel(Device::ADC_ch2);
                        curveCh2->setSamples(device.getADCBlock(delay));
                }
                else {
                        curveCh2->setVisible(false);
                }
        }
        // finally, refresh the plot
        ui->qwtPlot->replot();
}
