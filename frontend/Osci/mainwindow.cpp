#include <iostream>
#include <QFileDialog>
#include <QMessageBox>


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
        , delay(0)
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
        connect(&timeControl, SIGNAL(delayValueChanged(int)), this, SLOT(delayAdjusted(int)));

        connect(ui->actionExport_Data, SIGNAL(triggered()), this, SLOT(exportData()));

        statusBar()->showMessage("searching for device");
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

void MainWindow::delayAdjusted(int delay) {
        this->delay = delay;
}

void MainWindow::deviceConnected(bool connected) {
        statusBar()->showMessage(connected ? "connected" : "device not found");
        centralWidget()->setEnabled(connected);

        if (connected) {
                triggerControl.initialEmit();
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

static double fit_sin(double t, double p[3]) {
        return p[0] * sin ( p[1] + p[2]*t );
}



void lm_evaluate(double par[3], int m_dat, double fvec1[], void* obj, int *)
/*
 *	par is an input array. At the end of the minimization, it contains
 *        the approximate solution vector.
 *
 *	m_dat is a positive integer input variable set to the number
 *	  of functions.
 *
 *	fvec is an output array of length m_dat which contains the function
 *        values the square sum of which ought to be minimized.
 *
 *	data is a read-only pointer to lm_data_type, as specified by lm_eval.h.
 *
 *      info is an integer output variable. If set to a negative value, the
 *        minimization procedure will stop.
 */
{
        MainWindow* that = (MainWindow*) obj;
    for (int i = 0; i < m_dat; i++) {
        double t = that->fit_y[i] - fit_sin(that->fit_t[i], par);
        fvec1[i] = t;
    }
}

void MainWindow::sample() {

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

                                for (int i = 0; i< data.size(); i ++ ) {
                                        fit_t[i] = data.at(i).x();
                                        fit_y[i] = data.at(i).y()*100000.0;
                                }

                                // auxiliary settings:
        lm_control_type control;
        lm_initialize_control(&control);

                                control.maxcall = 3000;
                                control.ftol = 1e-20;
                                control.xtol = 1e-20;

                                lm_minimize(data.size(), 3, p_ch1,
                                            lm_evaluate, lm_print_default,
                                            (void*)this, &control);

                                //printf("values: %f  %f  %f\n", p_ch1[0],p_ch1[1],p_ch1[2]);
                                //channelControl1.setFitData( p_ch1[0],p_ch1[1],p_ch1[2]);
                                QVector<QPointF> dataFit;
                                for (int i = 0; i<data.size(); i ++ ) {
                                        dataFit.append(QPointF(fit_t[i], fit_sin(fit_t[i], p_ch1) / 100000.0));
                                }
                                curveFitSinCh1->setSamples(dataFit);
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
