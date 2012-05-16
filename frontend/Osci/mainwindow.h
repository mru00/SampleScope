#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMap>

#include "device.h"
#include "channelcontrol.h"
#include "triggercontrol.h"
#include "timecontrol.h"
#include "modecontrol.h"
#include "graphcontrol.h"
#include "measurementtablemodel.h"
#include "calibdialog.h"
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

    void deviceConnected(QString, QString);
    void deviceDisconnected();
    void fatal(QString message);

    void sample();
    void TriggerLevelAdjusted(int value);

    void VdivCh1Adjusted(DeviceConstants::VdivValues_t value);
    void VdivCh2Adjusted(DeviceConstants::VdivValues_t value);
    void ACDCCh1Adjusted(DeviceConstants::ACDC_t value);
    void ACDCCh2Adjusted(DeviceConstants::ACDC_t value);
    void triggerSourceSelected(DeviceConstants::TriggerSource_t);
    void triggerModeSelected(DeviceConstants::TriggerMode_t);
    void TdivAdjusted(DeviceConstants::TdivValues_t delay);
    void modeSelectionChanged(ModeControl::Modes_t);
    void TestSignalSelectionChanged(DeviceConstants::TestSignal_t);
    void updateSelectionChanged(ModeControl::Update_t);
    void singleShot();

    void graphSelectionChanged(QMap<GraphControl::Graphs_t,bool> enabled);
    void closeEvent(QCloseEvent *);
    void on_actionAbout_triggered();

    void on_actionUse_real_hardware_triggered();

    void on_actionUse_software_emulation_triggered();

    void on_actionDisconnect_triggered();

    void on_actionConnect_triggered();

    void on_actionAuto_connect_toggled(bool arg1);

private:
    Ui::MainWindow *ui;
    QTimer updateTimer, sampleTimer;
    Device device;

    Measurement meas;
    MeasurementTableModel model;
    ModeControl::Modes_t currentMode;
    CalibDialog calibDlg;

};

#endif // MAINWINDOW_H
