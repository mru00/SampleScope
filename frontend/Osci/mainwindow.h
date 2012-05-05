#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "device.h"
#include "channelcontrol.h"
#include "triggercontrol.h"
#include "timecontrol.h"
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
        void exportData();

private slots:

    void deviceConnected(bool connected);
    void fatal(QString message);

    void sample();
    void TriggerLevelAdjusted(int value);

    void VdivCh1Adjusted(QString hr, Device::VdivValues_t value);
    void VdivCh2Adjusted(QString hr, Device::VdivValues_t value);
    void ACDCCh1Adjusted(Device::ACDC_t value);
    void ACDCCh2Adjusted(Device::ACDC_t value);
    void triggerSourceSelected(Device::TriggerSource_t);
    void delayAdjusted(QString, Device::TdivValues_t delay);

private:
    Ui::MainWindow *ui;
    QTimer updateTimer;
    Device device;
    ChannelControl channelControl1, channelControl2;
    TriggerControl triggerControl;
    TimeControl timeControl;
    QwtPlotCurve *curveCh1, *curveCh2, *curveTriggerLevel, *curveFitSinCh1;
    QwtPlotGrid *plotGrid;

    Device::ACDC_t acdc_ch1, acdc_ch2;
    Device::VdivValues_t vdiv_ch1, vdiv_ch2;
    Device::TdivValues_t delay;

    double p_ch1[3];
};

#endif // MAINWINDOW_H
