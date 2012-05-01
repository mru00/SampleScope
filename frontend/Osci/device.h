#ifndef DEVICE_H
#define DEVICE_H

#include <vector>
#include <QObject>
#include <QPointF>
#include <QVector>
#include "Comm.h"
#include "../HIDAPI/hidapi.h"

#define MAX_STR 65

class Device : public QObject
{
        typedef float sample_t;
        Q_OBJECT

public:
        typedef unsigned short triggerLevel_t;
        enum Channel_t {
                ADC_ch1 = 0,
                ADC_ch2 = 1,
                ADC_triggerLevel = 2
        };
        enum TriggerSource_t {
                Trigger_Ch1,
                Trigger_Ch2,
                Trigger_Free
        };
        enum ACDC_t {
                ACDC_AC = 1,
                ACDC_DC = 0
        };
        enum VdivValues_t {
                Vdiv_10,
                Vdiv_5,
                Vdiv_2,
                Vdiv_1,
                Vdiv_05,
                Vdiv_02,
                Vdiv_01,
                Vdiv_005,
                Vdiv_LAST = Vdiv_005
        };

        enum {
                RX_SIZE = 10
        };
public:
        explicit Device(QObject *parent = 0);
        ~Device();

        bool isConnected() { return device != NULL; }
        void connect();
        void disConnect();



        void setVdiv(VdivValues_t gaina, VdivValues_t gainb);
        void ping();
        void setTriggerLevel(triggerLevel_t level);
        void selectChannel(Channel_t ch);
        sample_t getADCSingle();
        QVector<QPointF> getADCBlock(int delay = 0);
        void setACDC(ACDC_t ch1, ACDC_t ch2);
        void selectTriggerSource(TriggerSource_t trigger_source);
        TriggerSource_t getTriggerSource() { return triggerSource; }

        static QString getVdivLabel(VdivValues_t);
        static QString getChannelName(Channel_t);
signals:
        void connected(bool connected);
        void fatal(QString message);

public slots:
        void refresh();

private:
        TriggerSource_t triggerSource;
        hid_device* device;
        unsigned char buf[MAX_STR];

        sample_t normalizeSample(unsigned char val) {
                return 2.05 * (val - 128) / 128.0;
        }
        void comm(const unsigned char command);
};

#endif // DEVICE_H
