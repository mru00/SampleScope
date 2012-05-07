#ifndef TRIGGERCONTROL_H
#define TRIGGERCONTROL_H

#include <QWidget>

#include "device.h"

namespace Ui {
class TriggerControl;
}

class TriggerControl : public QWidget
{
    Q_OBJECT

public:
    explicit TriggerControl(QWidget *parent = 0);
    ~TriggerControl();
    void initialEmit();
private slots:
    void triggerCh1Toggled(bool);
    void triggerCh2Toggled(bool);
    void triggerFreeToggled(bool);

    void triggerRisingToggled(bool);
    void triggerFallingToggled(bool);
signals:
    void triggerSourceSelected(DeviceConstants::TriggerSource_t triggerSource);
    void triggerModeSelected(DeviceConstants::TriggerMode_t triggerMode);
private:
    Ui::TriggerControl *ui;
};

#endif // TRIGGERCONTROL_H
