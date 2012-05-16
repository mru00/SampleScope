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
