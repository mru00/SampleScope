#ifndef GRAPHCONTROL_H
#define GRAPHCONTROL_H

#include <QWidget>
#include <QMap>

namespace Ui {
class GraphControl;
}

class GraphControl : public QWidget
{
    Q_OBJECT

public:
    enum Graphs_t {
        Graph_Normal,
        Graph_XY,
        Graph_FFT,
        Graph_AutoCorr,
        Graph_Measurements,
        Graph_LAST = Graph_Measurements
    };

public:
    explicit GraphControl(QWidget *parent = 0);
    ~GraphControl();

    void initialEmit();
private slots:
        void itemSelectionChanged();
signals:
        void graphSelected(QMap<GraphControl::Graphs_t,bool>);
private:
    Ui::GraphControl *ui;
};

#endif // GRAPHCONTROL_H
