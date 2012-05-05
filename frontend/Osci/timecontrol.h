#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include <QWidget>
#include "device.h"

namespace Ui {
    class TimeControl;
}

class TimeControl : public QWidget
{
    Q_OBJECT

public:
    explicit TimeControl(QWidget *parent = 0);
    ~TimeControl();

        void initialEmit();
signals:
        void delayValueChanged(QString label, Device::TdivValues_t delay);
private slots:
        void delayvc(int value);
private:
    Ui::TimeControl *ui;
};

#endif // TIMECONTROL_H
