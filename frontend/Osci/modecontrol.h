#ifndef MODECONTROL_H
#define MODECONTROL_H

#include <QWidget>

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

public:
    explicit ModeControl(QWidget *parent = 0);
    ~ModeControl();

    void initialEmit();

    static bool showsCh1(Modes_t t) { return t == Mode_Ch1 || t == Mode_Interleaved || t == Mode_Sequential; }
    static bool showsCh2(Modes_t t) { return t == Mode_Ch2 || t == Mode_Interleaved || t == Mode_Sequential; }
private slots:
    void itemSelectionChanged();
signals:
    void modeSelected(ModeControl::Modes_t);
private:
    Ui::ModeControl *ui;
};

#endif // MODECONTROL_H
