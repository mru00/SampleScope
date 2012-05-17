#include "measurement.h"

#include <limits>
#include <cmath>
#include "device.h"
#include <fftw3.h>
#define BUG 5
#include "lmmin.h"
#include "lm_eval.h"

#include <QTextStream>

Measurement::Measurement(QObject *parent) :
    QObject(parent)
{
}


struct fit_data_t {
    double* t;
    double* y;
};

static double sin_par(double t, double p[3]) {
    return p[0] * sin ( p[1] + 2*M_PI*p[2]*t );
}

static void lm_evaluate(double par[3], int m_dat, double fvec1[], void* obj, int *)
{
    fit_data_t* that = (fit_data_t*) obj;
    for (int i = 0; i < m_dat; i++) {
        fvec1[i] = that->y[i] - sin_par(that->t[i], par);
    }
}

bool fitSin(double freq_guess, const QVector<QPointF>& samples, double p[], QVector<QPointF>& result) {

    result.clear();
    double fit_t[samples.size()];
    double fit_y[samples.size()];

    for (int i = 0; i< samples.size(); i ++ ) {
        fit_t[i] = samples.at(i).x();
        fit_y[i] = samples.at(i).y();
    }

    // auxiliary settings:
    lm_control_type control;
    lm_initialize_control(&control);

    fit_data_t fit_data;
    fit_data.t = fit_t;
    fit_data.y = fit_y;

    //control.maxcall = 3000;
    control.ftol = 1e-14;
    //control.xtol = 1e-20;

        p[2] = freq_guess;
    lm_minimize(samples.size(), 3, p,
                lm_evaluate, lm_print_default,
                &fit_data, &control);

    if (control.info != 1) {

        p[0] = 2.0;
        p[1] = 2.0;
        p[2] = freq_guess;

        return false;
    }


    for (int i = 0; i<samples.size(); i ++ ) {
        result.append(QPointF(fit_t[i], sin_par(fit_t[i], p) ));
    }
    return true;
}


static double rms(const QVector<QPointF>& data) {
    double sum = 0;
    for (int i = 0; i < data.size(); i++ ) {
        const double t = data.at(i).y();
        sum += t*t;
    }
    sum /= data.size();
    return sqrt(sum);
}

static double avg(const QVector<QPointF>& data) {
    double sum = 0;
    for (int i = 0; i < data.size(); i++ ) {
        sum += data.at(i).y();
    }
    sum /= data.size();
    return sum;
}

static void autocorrelation(
    int   n, double const x[],   /*  in: [0...n-1] samples x   */
    int lag, double       ac[])  /* out: [0...lag-1] ac values */
{
    double d; int i;
    while (lag--) {
        for (i = lag, d = 0; i < n; i++) {
            d += x[i] * x[i-lag];
        }
        ac[lag] = d;
    }
}


static int findGlobalMaximum(int count, double values[]) {
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

static int findSecondPeak(int count, double values[]) {

    const int win = 4;
    for (int i = win; i < count-win; i++ ) {
        double& current = values[i];
        bool foundABiggerOne = false;
        for (int k = i-win; k < i &&  !foundABiggerOne; k++ ) {
            if (values[k] >= current) foundABiggerOne = foundABiggerOne || true;
        }
        for (int k = i+1; k < i+win &&  !foundABiggerOne; k++ ) {
            if (values[k] >= current) foundABiggerOne = foundABiggerOne || true;
        }
        if (!foundABiggerOne) return i;

    }
    return -1;
}

static int findSecondPeak2(int count, double values[]) {

    const int win = 20;
    for (int i = win; i < count-win; i++ ) {

        bool acceptable = true;

        for (int k = 1; k < win && acceptable; k ++ ) {
            if (values[i-k] > values[(i-k)+1]) {
                acceptable = false;
            }
            if (values[i-k] < values[(i-k)+1]) break;
        }
        for (int k = 1; k < win && acceptable; k ++ ) {
            if (values[i+k] > values[i+k-1]) {
                acceptable = false;
            }
            if (values[i+k] < values[i+k-1]) break;
        }

        if (acceptable) return i;

    }
    return -1;
}



static double smooth(double arr[], int n, int i)
{
        double l,r,smoo;
        l = (i - 1 < 0)?arr[0]:arr[i-1];
        r = (i + 1 >= n)?arr[n-1]:arr[i+1];
        smoo = (l + 2*arr[i] + r)/4;
        return smoo;
}


// http://www.librow.com/articles/article-1
typedef double element;
//   1D MEDIAN FILTER implementation
//     signal - input signal
//     result - output signal
//     N      - length of the signal
static void _medianfilter(const element* signal, element* result, int N)
{
    const int winsize = 3;
   //   Move window through all elements of the signal
   for (int i = 2; i < N - 2; ++i)
   {
      //   Pick up window elements
      element window[winsize];
      for (int j = 0; j < winsize; ++j)
         window[j] = signal[i - (winsize/2) + j];
      //   Order elements (only half of them)
      for (int j = 0; j < (winsize/2); ++j)
      {
         //   Find position of minimum element
         int min = j;
         for (int k = j + 1; k < winsize; ++k)
            if (window[k] < window[min])
               min = k;
         //   Put found minimum element in its place
         const element temp = window[j];
         window[j] = window[min];
         window[min] = temp;
      }
      //   Get result - the middle element
      result[i - winsize/2] = window[winsize/2];
   }
}

// http://stackoverflow.com/questions/3242910/algorithm-to-locate-local-maxima
static int findmax(double arr[], int n)
{
    double filtered[n];
    _medianfilter(arr, filtered, n);

        double prev = filtered[0];
        int i;
        int goup = 1;
        int has_crossed_zero = 0;

        for(i =1; i < n; i++)
        {
                //double cur = smooth(arr,n,i);
            double cur = filtered[i];

            if (arr[i] < 0) has_crossed_zero = 1;

                if(goup) {
                        if(prev > cur && i > 0) {
                                if (has_crossed_zero == 1 && cur > 0) {
                                    return i;
                                }
                                goup = 0;
                        }
                } else {
                        if(prev < cur)
                                goup = 1;
                }
                prev = cur;
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

void Measurement::ChannelData::setData(const DataSeries& data) {
    clear();

    samples = data;
    const int inputSize = samples.size();

    if (inputSize == 0) return;

    double _acorr[inputSize*2];

    double freq_guess = 0.02;
    double vals[inputSize];
    double fftMag[inputSize];
    fftw_complex out[inputSize];

    fftw_plan plan = fftw_plan_dft_r2c_1d(inputSize, vals, out, 0);

    int i;
    // prepare input
    for (i = 0; i < inputSize; i ++ )
        vals[i] = samples.at(i).y();

    autocorrelation(inputSize, vals, inputSize, _acorr);

    // lag in samples, where the second peak occurs
    apeak_pos = findmax(_acorr, inputSize);
    if (apeak_pos != -1 ) {
        double samplePerDiv = (inputSize/10.0);
        double lagTime = Device::getTdivTime(tdiv, apeak_pos) / samplePerDiv;
        apeak = 1/lagTime;
        ArrToQVector(inputSize, _acorr, acorr);

        freq_guess = 0.1 * (double)inputSize/(double)apeak_pos;
    }

    fftw_execute(plan);

    fftToMagnitude(out, fftMag, inputSize/2);
    fpeak_pos = findGlobalMaximum(inputSize/2, fftMag);
    if (fpeak_pos != -1) {
        ArrToQVector(inputSize/2, fftMag, fft);
        double fsamp = inputSize / ( 10 * Device::getTdivTime(tdiv, 1) );
        fpeak = fpeak_pos * fsamp / inputSize;
    }

    fftw_destroy_plan(plan);

    rms_div = ::rms(samples);
    avg_div = ::avg(samples);
    rms_volt = Device::getVdivVoltate(vdiv, rms_div);
    avg_volt = Device::getVdivVoltate(vdiv, avg_div);

    if (fitSin(freq_guess, samples, fit_p, fittedSine)) {
        speak = 1 /  Device::getTdivTime(tdiv, 1/fit_p[2]);
    }
    else {
        speak = std::numeric_limits<double>::quiet_NaN();
    }
}

void Measurement::setChannelData(const DataSeries& dataCh1, const DataSeries& dataCh2) {
    channelData[0].setData(dataCh1);
    channelData[1].setData(dataCh2);
}

void Measurement::setTdiv(DeviceConstants::TdivValues_t val) {
    channelData[0].tdiv = val;
    channelData[1].tdiv = val;
}

void Measurement::setVdiv_Ch1(DeviceConstants::VdivValues_t val) {
    channelData[0].vdiv = val;
}

void Measurement::setVdiv_Ch2(DeviceConstants::VdivValues_t val) {
    channelData[1].vdiv = val;
}

const Measurement::ChannelData& Measurement::getChannel(DeviceConstants::Channel_t ch) const {
    switch (ch) {
    case DeviceConstants::ADC_ch1:
        return channelData[0];
    default:
        Q_ASSERT(ch == DeviceConstants::ADC_ch2);
        return channelData[1];
    }
}

static QTextStream& join(const Measurement::DataSeries& data, QTextStream & str) {
    const char* nl = "\n";
    str << "[";
    for (int i = 0; i < data.size(); i ++ ) {
        str << "    " << data.at(i).x() << " " << data.at(i).y() << nl;
    }
    str << "]";
    return str;
}

void Measurement::writeMatlab(QFile &f) {
    QTextStream stream (&f);
    const char* nl = "\n";
    stream << "# sample scope data dump" << nl
        << nl;
    for ( int i = 0; i < 2; i++ ) {
        const ChannelData& ch = channelData[i];

#define acc "data(" << i+1 << ")"

        stream << acc << ".rms_div = " << ch.rms_div << ";" << nl
                  << acc << ".rms_volt = " << ch.rms_volt << ";" << nl
                  << acc << ".avg_div = " << ch.avg_div << ";" << nl
                  << acc << ".avg_volt = " << ch.avg_volt << ";" << nl
                  << acc << ".vdiv = " << ch.vdiv << ";" << nl
                  << acc << ".volt_per_div = " << Device::getVdivVoltate(ch.vdiv,1) << ";" << nl
                  << acc << ".sin_par = [" << ch.fit_p[0] << " " << ch.fit_p[1] << " " << ch.fit_p[2] << "];" << nl
                  << acc << ".apeak_pos = " << ch.apeak_pos << ";" << nl
                  << acc << ".fpeak_pos = " << ch.fpeak_pos << ";" << nl
                  << acc << ".apeak = " << ch.apeak << ";" << nl
                  << acc << ".fpeak = " << ch.fpeak << ";" << nl
                  << acc << ".speak = " << ch.speak << ";" << nl;


        stream << acc << ".samples = " ;
        join (ch.samples, stream);
        stream << ";" << nl;

        stream << acc << ".fft = " ;
        join (ch.fft, stream);
        stream << ";" << nl;

        stream << acc << ".acorr = " ;
        join (ch.acorr, stream);
        stream << ";" << nl;

        stream << acc << ".fittedSine = " ;
        join (ch.fittedSine, stream);
        stream << ";" << nl;

    }
}
