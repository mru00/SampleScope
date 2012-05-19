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


#ifndef MEASUREMENTTABLEMODEL_H
#define MEASUREMENTTABLEMODEL_H

#include <limits>
#include <QAbstractTableModel>
#include <QVector>
#include <QPointF>

#include "measurement.h"

#include "../../shared/common.h"


class MeasurementTableModel : public QAbstractTableModel
{
    Q_OBJECT

    enum {
        Meas_RMS_div,
        Meas_RMS_Volt,
        Meas_AVG_div,
        Meas_AVG_Volt,
        //Meas_Freq_FFT,
        //Meas_Freq_ACORR,
        Meas_Freq_Sin,
        Meas_END
    } Measurements_t;

public:
    explicit MeasurementTableModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;


    void setData(const Measurement& meas);
signals:

public slots:

public:
    QVariant items[2][Meas_END];
};

#endif // MEASUREMENTTABLEMODEL_H
