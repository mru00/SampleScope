#ifndef MODECONTROL_H
#define MODECONTROL_H

#include <QWidget>
#include "../../shared/common.h"

namespace Ui {
class ModeControl;
}

class ModeControl : public QWidget
{
    Q_OBJECT
public:
    enum Modes_t {
        Mode_Ch1,
        Mode_Ch2,
        Mode_Interleaved,
        Mode_Sequential,
        Mode_LAST = Mode_Sequential
    };

    enum Update_t {
        Update_Continuous,
        Update_Single,
        Update_LAST = Update_Single
    };

public:
    explicit ModeControl(QWidget *parent = 0);
    ~ModeControl();

    void initialEmit();

    static bool showsCh1(Modes_t t) { return t == Mode_Ch1 || t == Mode_Interleaved || t == Mode_Sequential; }
    static bool showsCh2(Modes_t t) { return t == Mode_Ch2 || t == Mode_Interleaved || t == Mode_Sequential; }
private slots:
    void itemSelectionChanged();
    void testSignalSelectionChanged();
    void continuousToggled(bool);
    void singleToggled(bool);
    void singleShotClicked();

signals:
    void testSignalSelected(DeviceConstants::TestSignal_t);
    void modeSelected(ModeControl::Modes_t);
    void updateSelected(ModeControl::Update_t);
    void singleShot();
private:
    Ui::ModeControl *ui;
};

#endif // MODECONTROL_H
