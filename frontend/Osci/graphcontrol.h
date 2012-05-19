/*

SampleScope: A humble USB oscilloscope
Copyright (C) 2012 mru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
