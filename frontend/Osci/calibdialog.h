#ifndef CALIBDIALOG_H
#define CALIBDIALOG_H

#include <QDialog>

class Device;

namespace Ui {
    class CalibDialog;
}

class CalibDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibDialog(Device* dev, QWidget *parent = 0);
    virtual ~CalibDialog();

    void setDelay(unsigned short delay);

    private slots:
    void delayChanged(int value);
private:
    Ui::CalibDialog *ui;
    Device* device;
};

#endif // CALIBDIALOG_H
