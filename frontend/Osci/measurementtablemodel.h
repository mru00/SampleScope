#ifndef MEASUREMENTTABLEMODEL_H
#define MEASUREMENTTABLEMODEL_H

#include <limits>
#include <QAbstractTableModel>
#include <QVector>
#include <QPointF>

#include "../../shared/common.h"


class MeasurementTableModel : public QAbstractTableModel
{
    Q_OBJECT
    class Data {
    public:
        Data() : vdiv (DeviceConstants::Vdiv_005) {
            clear();
        }
        QVector<QPointF> fft;
        QVector<QPointF> acorr;
        double rms, avg, fpeak, apeak;
        int apeak_pos, fpeak_pos;
        void clear() {
            rms = avg = fpeak = apeak = std::numeric_limits<double>::quiet_NaN();
            fft.clear();
            acorr.clear();
        }
        DeviceConstants::VdivValues_t vdiv;
    };

    enum {
        Meas_RMS_div,
        Meas_RMS_Volt,
        Meas_AVG_div,
        Meas_AVG_Volt,
        Meas_Freq_FFT,
        Meas_Freq_ACORR,
        Meas_LAST = Meas_Freq_ACORR
    } Measurements_t;

public:
    explicit MeasurementTableModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    QVector<QPointF> getFFT(DeviceConstants::Channel_t);
    QVector<QPointF> getAutoCorr(DeviceConstants::Channel_t);
    int getAutoCorrMaxIdx(DeviceConstants::Channel_t);

    void setChannelData(const QVector<QPointF>& ch1, const QVector<QPointF>& ch2);
signals:

public slots:

    void setTdiv(DeviceConstants::TdivValues_t);
    void setVdiv_Ch1(DeviceConstants::VdivValues_t);
    void setVdiv_Ch2(DeviceConstants::VdivValues_t);

private:
    DeviceConstants::TdivValues_t tdiv;
    Data channelData[2];
    Data& getChannel(DeviceConstants::Channel_t);
};

#endif // MEASUREMENTTABLEMODEL_H
