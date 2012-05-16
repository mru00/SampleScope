#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QObject>
#include <QFile>
#include <limits>
#include <QVector>
#include <QPointF>

#include "../../shared/common.h"

class Measurement : public QObject
{
    Q_OBJECT
public:
    typedef QVector<QPointF> DataSeries;

    class ChannelData {
    public:
        ChannelData() : vdiv (DeviceConstants::Vdiv_005), tdiv(DeviceConstants::Tdiv_01) {
            clear();
        }
        DataSeries fft, acorr, samples, fittedSine;
        double rms_div, rms_volt, avg_div, avg_volt, fpeak, apeak, speak;
        int apeak_pos, fpeak_pos;
        void clear() {
            apeak_pos = fpeak_pos = -1;
            rms_div = rms_volt =
                    avg_div = avg_volt =
                    fpeak = apeak = speak =
                    std::numeric_limits<double>::quiet_NaN();
            fft.clear();
            acorr.clear();
            samples.clear();
            fittedSine.clear();
        }
        DeviceConstants::VdivValues_t vdiv;
        DeviceConstants::TdivValues_t tdiv;
        double fit_p[3];
        double min_y(const DataSeries& series) {
            double min = std::numeric_limits<double>::max();
            for (int i = 0; i< series.size(); i++) {
                if (series.at(i).y() < min) min = series.at(i).y();
            }
            return min;
        }
        double max_y(const DataSeries& series) {
            double max = std::numeric_limits<double>::min();
            for (int i = 0; i < series.size(); i ++ ) {
                if (series.at(i).y() > max) max = series.at(i).y();
            }
            return max;
        }
        void setData(const DataSeries& data);
    };
    explicit Measurement(QObject *parent = 0);
    void setChannelData(const DataSeries& dataCh1, const DataSeries& dataCh2);

    const ChannelData& getChannel(DeviceConstants::Channel_t) const;
    const ChannelData& getChannel1() const { return getChannel(DeviceConstants::ADC_ch1); }
    const ChannelData& getChannel2() const { return getChannel(DeviceConstants::ADC_ch2); }

    void writeMatlab(QFile& f);
signals:
    void measurementsUpdated();

public slots:
    void setTdiv(DeviceConstants::TdivValues_t);
    void setVdiv_Ch1(DeviceConstants::VdivValues_t);
    void setVdiv_Ch2(DeviceConstants::VdivValues_t);
private:
    ChannelData channelData[2];
};

#endif // MEASUREMENT_H
