#include "measurementtablemodel.h"
#include "device.h"
#include <cmath>

#include <fftw3.h>

MeasurementTableModel::MeasurementTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    for (int i = 0; i <2; i ++ ) {
        for ( int j = 0; j < 20; j++ ) {
            createIndex(i,j, i*100+j);
        }
    }
}

static QVariant checkNaN(double n) {
    if (n == std::numeric_limits<double>::quiet_NaN())
        return QVariant::Invalid;
    return n;
}

QVariant MeasurementTableModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case Qt::DisplayRole: {

        const Data& currentChannel = channelData[index.column()];
        switch (index.row()) {
        case Meas_RMS_div:
            return checkNaN(currentChannel.rms);
        case Meas_RMS_Volt:
            return checkNaN(Device::getVdivVoltate( currentChannel.vdiv, currentChannel.rms));
        case Meas_AVG_div:
            return checkNaN(currentChannel.avg);
        case Meas_AVG_Volt:
            return checkNaN(Device::getVdivVoltate(currentChannel.vdiv, currentChannel.avg));
        case Meas_Freq_FFT:
            return checkNaN(currentChannel.fpeak);
        case Meas_Freq_ACORR:
            return checkNaN(currentChannel.apeak);
        }
    }
    break;
    case Qt::DecorationRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
    case Qt::StatusTipRole:
    case Qt::WhatsThisRole:
    case Qt::SizeHintRole:
    case Qt::FontRole:
    case Qt::TextAlignmentRole:
    case Qt::BackgroundRole:
        //case Qt::BackgroundColorRole:
    case Qt::ForegroundRole:
        //case Qt::TextColorRole:
    case Qt::CheckStateRole:
    default:
        return QVariant::Invalid;
    }
    return QVariant::Invalid;
}
int MeasurementTableModel::rowCount(const QModelIndex &parent) const {
    return 6;
}

int MeasurementTableModel::columnCount(const QModelIndex &parent) const {
    return 2;
}
QVariant MeasurementTableModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    QVariant data;

    const char* horiz[] = {
        "Ch1", "Ch2"
    };

    const char* vert[] = {
        "rms [div]",
        "rms [V]",
        "avg [div]",
        "avg [V]",
        "f [fft_idx]",
        "f [acorr]"
    };

    switch (orientation) {
    case Qt::Vertical:
        data = QString(vert[section]);
        break;
    case Qt::Horizontal:
        data = QString(horiz[section]);
        break;
    }

    switch (role) {
    case Qt::DisplayRole:
        return data;
        break;
    case Qt::DecorationRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
    case Qt::StatusTipRole:
    case Qt::WhatsThisRole:
    case Qt::SizeHintRole:
    case Qt::FontRole:
    case Qt::TextAlignmentRole:
    case Qt::BackgroundRole:
        //case Qt::BackgroundColorRole:
    case Qt::ForegroundRole:
        //case Qt::TextColorRole:
    case Qt::CheckStateRole:
    default:
        return QVariant::Invalid;
    }
    return QVariant::Invalid;
}

static double rms(const QVector<QPointF>& data) {
    double sum = 0;
    for (int i = 0; i < data.size(); i++ ) {
        double t = data.at(i).y();
        sum += t*t;
    }
    sum /= data.size();
    return sqrt(sum);
}

static double avg(const QVector<QPointF>& data) {
    double sum = 0;
    for (int i = 0; i < data.size(); i++ ) {
        double t = data.at(i).y();
        sum += t;
    }
    sum /= data.size();
    return sum;
}

void autocorrelation(
    int   n, double const x[],   /*  in: [0...n-1] samples x   */
    int lag, double       ac[])  /* out: [0...lag-1] ac values */
{
    double d; int i;
    while (lag--) {
        for (i = lag, d = 0; i < n; i++) {
            Q_ASSERT(i>=0 && i< 256);
            Q_ASSERT(i-lag >= 0 && i-lag < 256);
            d += x[i] * x[i-lag];
        }
        ac[lag] = d;
    }
}


int findGlobalPeak(int count, double values[]) {
    double peak = std::numeric_limits<double>::min();
    int peak_pos = -1;
    for (int i = 0; i < count; i ++ ) {
        if (values[i] > peak) {
            peak = values[i];
            peak_pos = i;
        }
    }
    return peak_pos;
}

int findSecondPeak(int count, double values[]) {

    const int win = 3;
    for (int i = win; i < count-win; i++ ) {
        double& current = values[i];
        bool foundABiggerOne = false;
        for (int k = i-win; k < i &&  !foundABiggerOne; k++ ) {
           if (values[k] > current) foundABiggerOne = foundABiggerOne || true;
        }
         for (int k = i+1; k < i+win &&  !foundABiggerOne; k++ ) {
           if (values[k] > current) foundABiggerOne = foundABiggerOne || true;
        }
         if (!foundABiggerOne) return i;

    }
    return -1;
}

static void ArrToQVector(int count, double data[], QVector<QPointF>& result) {
    result.clear();
    for ( int i = 0; i < count; i ++ ) {
        result.append(QPointF(i, data[i]));
    }
}

static void fftToMagnitude(fftw_complex input[], double output[], int count) {
#define NORM(x,y) sqrt( (x)*(x) + (y)*(y) )
    for (int i = 0; i < count; i ++ ) {
        output[i] = NORM(input[i][0], input[i][1]);
    }
}

void MeasurementTableModel::setChannelData(const QVector<QPointF>& dataCh1, const QVector<QPointF>& dataCh2) {


    // should be array of references...
    const QVector<QPointF> input [] = { dataCh1, dataCh2 };



    for ( int ch = 0; ch < 2; ch ++ ) {

        Data& currentChannel = channelData[ch];
        currentChannel.clear();

        const int inputSize = input[ch].size();

        if (inputSize > 0) {


            double acorr[inputSize*2];

            double vals[inputSize];
            double fftMag[inputSize];
            fftw_complex out[inputSize];

            fftw_plan plan = fftw_plan_dft_r2c_1d(inputSize, vals, out, 0);

            int i;
            // prepare input
            for (i = 0; i < inputSize; i ++ )
                vals[i] = input[ch].at(i).y();

            autocorrelation(inputSize, vals, inputSize, acorr);

            // lag in samples, where the second peak occurs
            currentChannel.apeak_pos = findSecondPeak(inputSize, acorr);
            if (currentChannel.apeak_pos != -1 ) {
                // there are 25.6 samples in one div
                double lagTime = Device::getTdivTime(tdiv, currentChannel.apeak_pos) / 25.6;
                currentChannel.apeak = 1/lagTime;
                ArrToQVector(inputSize, acorr, currentChannel.acorr);
            }

            fftw_execute(plan);

            fftToMagnitude(out, fftMag, inputSize/2);
            currentChannel.fpeak_pos = findGlobalPeak(inputSize/2, fftMag);
            if (currentChannel.fpeak_pos != -1) {
                ArrToQVector(inputSize/2, fftMag, currentChannel.fft);
                double fsamp = inputSize / ( 10 * Device::getTdivTime(tdiv, 1) );
                currentChannel.fpeak = currentChannel.fpeak_pos * fsamp / inputSize;
            }

            fftw_destroy_plan(plan);

            currentChannel.rms = ::rms(input[ch]);
            currentChannel.avg = ::avg(input[ch]);
        }
    }

    const QModelIndex& i0 = index(0,0);
    const QModelIndex& i1 = index(rowCount(QModelIndex())-1, columnCount(QModelIndex())-1);

    emit dataChanged(i0,i1);
}

QVector<QPointF> MeasurementTableModel::getAutoCorr(DeviceConstants::Channel_t ch) {
    return getChannel(ch).acorr;
}
QVector<QPointF> MeasurementTableModel::getFFT(DeviceConstants::Channel_t ch) {
    return getChannel(ch).fft;
}
void MeasurementTableModel::setTdiv(DeviceConstants::TdivValues_t val) {
    tdiv = val;
}
void MeasurementTableModel::setVdiv_Ch1(DeviceConstants::VdivValues_t val) {
    channelData[0].vdiv = val;
}
void MeasurementTableModel::setVdiv_Ch2(DeviceConstants::VdivValues_t val) {
    channelData[1].vdiv = val;
}
MeasurementTableModel::Data& MeasurementTableModel::getChannel(DeviceConstants::Channel_t ch) {
    switch (ch) {
    case DeviceConstants::ADC_ch1:
        return channelData[0];
    case DeviceConstants::ADC_ch2:
        return channelData[1];
    }
}
