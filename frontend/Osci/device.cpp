#include <stdio.h>
#include <iostream>

#include "device.h"
#include "../../shared/common.h"


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
    InitOsciConfig(config);
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

    nread = hid_read_timeout(device, buf, sizeof(buf), 10000) ;
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
    comm(DeviceConstants::OP_PING);
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


QString Device::getChannelName(DeviceConstants::Channel_t val) {
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
        4095, // 0xfff
        2048, // 0x800
        819,  // 0x333
        410,  // 0x19a
        205,  // 0xcd
        82,   // 0x52
        41,   // 0x29
        20    // 0x14
    };

    Q_ASSERT (gaina >=0 && gaina < sizeof(dacvals));
    Q_ASSERT (gainb >=0 && gainb < sizeof(dacvals));

    config.gainCh1 = dacvals[gaina];
    config.gainCh2 = dacvals[gainb];
    transmitConfig();
}

void Device::setTriggerLevel(const triggerLevel_t level) {
    config.triggerLevel = level;
    transmitConfig();
}

void Device::selectChannel(const DeviceConstants::Channel_t ch) {
    config.inputChannel = ch;
    transmitConfig();
}

Device::sample_t Device::getADCSingle() {
    comm(DeviceConstants::OP_READ_SINGLE);
    return normalizeSample(buf[1]);
}

QVector<QPointF> Device::getADCBlock(int delay) {
    delay = 0;
    buf[2] = delay;
    cout << "sampling with delay: " << delay << endl;
    comm ( (delay == 0) ? DeviceConstants::OP_SAMPLE_FASTEST_SINGLE : DeviceConstants::OP_SAMPLE_DELAYED_SINGLE);

    QVector<QPointF> result;
    // comm might have failed and closed the device
    if (!isConnected()) return result ;



    // 10 divs for 191 samples
    const double timestretch = 10.0/191.0;

    for (size_t i = 1; i < 64; i++ ) {
        result.append(QPointF( timestretch * (i-1), normalizeSample(buf[i])));
    }
    for (int j = 0; j < 2; j++ ) {

        hid_read(device, buf, sizeof(buf)) ;
        for (size_t i = 0; i < 64; i++ ) {
            result.append(QPointF(timestretch*(i + 63 + j*64), normalizeSample(buf[i])));
        }
    }
    cout << "read data from ADC" << endl;
    return result;
}

void Device::getADCInterleaved(int delay, QVector<QPointF>& ch1, QVector<QPointF>& ch2) {
    delay = 0;
    buf[2] = delay;
    cout << "sampling with delay: " << delay << endl;
    comm ( DeviceConstants::OP_SAMPLE_FASTEST_INTERLEAVED);

    QVector<QPointF> result;
    // comm might have failed and closed the device
    if (!isConnected()) return;



    // 10 divs for 191 samples
    const double timestretch = 10.0/191.0;

    ch1.clear();
    ch2.clear();

    for (size_t i = 2; i < 64; i++ ) {
        const QPointF point(timestretch * ((i/2) - 1), normalizeSample(buf[i]));
        if (i&1) ch2.append(point);
        else ch1.append(point);
    }
    for (int j = 0; j < 2; j++ ) {

        hid_read(device, buf, sizeof(buf)) ;
        for (size_t i = 0; i < 64; i++ ) {
            const QPointF point(timestretch * (i/2 + 31 + j*32), normalizeSample(buf[i]));
            if (i&1) ch2.append(point);
            else ch1.append(point);
        }
    }
    cout << "read data from ADC, interleaved" << endl;
    return;
}


void Device::setACDC_Ch1(DeviceConstants::ACDC_t ch1) {
    config.acdcCh1 = ch1;
    transmitConfig();
}

void Device::setACDC_Ch2(DeviceConstants::ACDC_t ch2) {
    config.acdcCh2 = ch2;
    transmitConfig();
}

void Device::selectTriggerSource(DeviceConstants::TriggerSource_t trigger_source) {
    cout << "selecting trigger source " << trigger_source << endl;
    config.triggerChannel = trigger_source;
    transmitConfig();
}

void Device::transmitConfig() {
    cout << "transmitting new config" << endl;
    memcpy(&buf[2], &config, sizeof(DeviceConstants::osci_config_t));
    comm(DeviceConstants::OP_SET_CONFIG);
    /*
    DeviceConstants::osci_config_t newconf;
    int sizefw = buf[1];
    if (sizefw != sizeof(DeviceConstants::osci_config_t)) {
        emit fatal("wrong config size");
    }
    memcpy(&newconf, &buf[2], sizeof(DeviceConstants::osci_config_t));
    int cmp = memcmp(&config, &newconf, sizeof(DeviceConstants::osci_config_t));
    if (cmp != 0 ) {
        emit fatal("wrong answer on transmit config");
    }
    */
}
