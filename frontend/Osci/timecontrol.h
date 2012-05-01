#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include <QWidget>

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
        void delayValueChanged(int delay);
private slots:
        void delayvc(int value);
private:
    Ui::TimeControl *ui;
};

#endif // TIMECONTROL_H
