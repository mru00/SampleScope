#include <stdio.h>
#include <iostream>

#include "device.h"



#define VENDOR_ID 0x04d8
#define PRODUCT_ID 0x003f


using namespace std;


union WORD_VAL {
        unsigned char b[2];
        unsigned short val;
        WORD_VAL(unsigned short v) : val(v) {}
} __attribute__((packed));

Device::Device(QObject *parent) :
        QObject(parent),
        device(NULL)
{
}

Device::~Device() {
        hid_close(device);
        device = NULL;
}

void Device::refresh() {
        if (!isConnected()) connect();

        if (isConnected()) ping();
}

void Device::connect() {
        cout << "connecting to device" << endl;
        device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
        cout << (isConnected() ? "connection successful" : "connection failed") << endl;
        if (isConnected()) {

                wchar_t manu[100], prod[100];
                hid_get_manufacturer_string(device, manu, 100);
                hid_get_product_string(device, prod, 100);
                std::wcout << manu << endl;
                std::wcout << prod << endl;
        }
        emit connected(isConnected());
}

void Device::disConnect() {
        cout << "disconnecting from device" << endl;
        hid_close(device);
        device = NULL;
        emit connected(false);
}

void Device::comm(const unsigned char command) {
        cerr << "sending command " << hex << showbase << (int)command << endl;
        if (!isConnected()) {
                cerr << "tying to communicate with the device, but it is not connected" << endl;
                emit fatal("trying to communicate with the device, but is is not connected");
                return;
        }
        int nread;
        buf[0] = 0x00;
        buf[1] = command;
        nread = hid_write(device, buf, sizeof(buf));
        if (nread < 0) {
                cerr << "hid_write failed, retcode: " << nread << endl;
                disConnect();
                emit fatal(tr("Error communicating with device (hid_write failed); disconnected device"));
                return;
        }

        nread = hid_read(device, buf, sizeof(buf)) ;
        if( nread != 64) {
                cerr << "hid_read failed, return size = " << nread << endl;
                disConnect();
                emit fatal(tr("Failed to read from device"));
                return;
        }

        if (buf[0] != command+1) {
                cout << "device sent unexpected answer, expected = " << command+1 << " received = " << buf[0] << endl;
                disConnect();
                //printf("sent: 0x%02x, received: 0x%02x\n", command, buf[0]);
                emit fatal(tr("Unexpected Answer from device"));
                return;
        }
}

void Device::ping() {
        comm(0x90);
}

QString Device::getTdivUnit(TdivValues_t val) {
        return (val < Tdiv_1 ? "usec/div" : "msec/div");
}
QString Device::getTdivLabel(TdivValues_t val) {
        const char* labels[] = {
                "12.8",
                "30",
                "100",
                "300",
                "1",
                "3",
                "10",
                "30",
                "100",
                "300",
                "1000",
                "3000"
        };
        Q_ASSERT ( val >= 0 && val < sizeof(labels) );

        return labels[val];
}

QString Device::getVdivLabel(VdivValues_t val) {

        const char* labels[] = {
                "10", "5", "2", "1", "0.5", "0.2", "0.1", "0.05"
        };
        Q_ASSERT ( val >= 0 && val < sizeof(labels) );
        return labels[val];
}


QString Device::getChannelName(Channel_t val) {
        const char* names[] = {
                "Channel 1",
                "Channel 2",
                "Trigger Level"
        };
        Q_ASSERT ( val >= 0 && val < sizeof(names) );
        return names[val];
}

void Device::setVdiv(VdivValues_t gaina, VdivValues_t gainb) {
        cout << "setting gain to " << gaina << " / " << gainb << endl;

        unsigned short dacvals[] = {
                4095,
                2048,
                819,
                410,
                205,
                82,
                41,
                20
        };

        Q_ASSERT (gaina >=0 && gaina < sizeof(dacvals));
        Q_ASSERT (gainb >=0 && gainb < sizeof(dacvals));

        WORD_VAL daca = dacvals[gaina], dacb = dacvals[gainb];

        buf[2] = dacb.b[0];
        buf[3] = dacb.b[1];

        buf[4] = daca.b[0];
        buf[5] = daca.b[1];
        comm(0x30);
}

void Device::setTriggerLevel(triggerLevel_t level) {
        WORD_VAL l = level;
        buf[2] = l.b[0];
        buf[3] = l.b[1];
        comm(0x20);
}

void Device::selectChannel(Channel_t ch) {
        buf[2] = ch & 0xff;
        comm(0x50);
}

Device::sample_t Device::getADCSingle() {
        comm(0x40);
        return normalizeSample(buf[1]);
}

QVector<QPointF> Device::getADCBlock(int delay) {
        delay = 0;
        buf[2] = delay;
        cout << "sampling with delay: " << delay << endl;
        comm ( (delay == 0) ? 0x60 : 0x70);
        QVector<QPointF> result;
        const double timestretch = 1.0/19.2;
        for (size_t i = 1; i < 64; i++ ) {
                result.append(QPointF( timestretch * (i-1), normalizeSample(buf[i])));
        }
        for (int j = 0; j < 2; j++ ) {

                hid_read(device, buf, sizeof(buf)) ;
                for (size_t i = 0; i < 64; i++ ) {
                        result.append(QPointF(timestretch*(i + 63 + j*64), normalizeSample(buf[i])));
                }
        }
        return result;
}


void Device::setACDC(ACDC_t ch1, ACDC_t ch2) {
        unsigned char vch1 = (ch1 == ACDC_AC) ? 1 : 0;
        unsigned char vch2 = (ch2 == ACDC_AC) ? 1 : 0;
        buf[2] = (vch1 & 1) | ((vch2 & 1) << 1);
        comm(0x10);
}

void Device::selectTriggerSource(TriggerSource_t trigger_source) {
    cout << "selecting trigger source " << trigger_source << endl;
    this->triggerSource = trigger_source;
    buf[2] = trigger_source;
    comm(0x80);
}
