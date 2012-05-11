#include <stdio.h>
#include <iostream>

#include "device.h"
#include "../../shared/common.h"


using namespace std;

Device::Device(QObject *parent) :
    QObject(parent),
    device(AbstractHardware::create(AbstractHardware::Impl_Real)),
    dummy(DeviceConstants::TestSignal_Tri)
{

        //"12.8", "30", "100", "300",
        //"1", "3", "10", "30", "100", "300", "1000", "3000"
    unsigned short delays[] = {

       0,// fastest
        6,// 100 u
        37,// 300 u
        147,// 1m
        454,// 3m
        1549,// 10m
        4600,// 30m
        15000,// 100m

        (187500-16)/9,
        (625000-16)/9,
        (1875000-16)/9,
    };
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

    for ( int i = 0; i <= DeviceConstants::Vdiv_LAST; i ++ ) gain_ch1_cal[i] = dacvals[i];
    for ( int i = 0; i <= DeviceConstants::Vdiv_LAST; i ++ ) gain_ch2_cal[i] = dacvals[i];
    for ( int i = 0; i <= DeviceConstants::Tdiv_LAST; i ++ ) delay_cal[i] = delays[i];
    InitOsciConfig(config);
}

Device::~Device() {
    device->close();
    delete device;
}



void Device::selectHardwareImplementation(AbstractHardware::Impl_t impl) {

    if (impl == device->getImpl()) return;

    if (device->isOpen()) disConnect();
    delete device;
    device = AbstractHardware::create(impl);
}


void Device::refresh() {
    if (!isConnected()) connect();
    if (isConnected()) ping();
}

void Device::connect() {
    cout << "connecting to device" << endl;
    device->open();
    cout << (isConnected() ? "connection successful" : "connection failed") << endl;
    if (isConnected()) {

        wchar_t manu[100], prod[100];
        device->get_manufacturer_string(manu, 100);
        device->get_product_string(prod, 100);
        std::wcout << manu << endl;
        std::wcout << prod << endl;
        QString qmanu = QString::fromStdWString(manu);
        QString qprod = QString::fromStdWString(prod);

        emit connected(qmanu, qprod);
    }
}

void Device::disConnect() {
    cout << "disconnecting from device" << endl;
    device->close();
    emit disconnected();
}

void Device::setTestSignal(DeviceConstants::TestSignal_t dummy) {
    this->dummy = dummy;
}

void Device::comm(const DeviceConstants::opcodes_t command) {
    cerr << "sending command " << hex << showbase << (int)command << endl;
    if (!isConnected()) {
        cerr << "tying to communicate with the device, but it is not connected" << endl;
        emit fatal("trying to communicate with the device, but is is not connected");
        return;
    }
    int nread;
    buf[0] = 0x00;
    buf[1] = command;
    nread = device->write(buf, sizeof(buf));
    if (nread < 0) {
        cerr << "hid_write failed, retcode: " << nread << endl;
        disConnect();
        emit fatal(tr("Error communicating with device (hid_write failed); disconnected device"));
        return;
    }

    nread = device->read(buf, 64);
    if( nread != 64) {
        cerr << "hid_read failed, return size = " << nread << endl;
        disConnect();
        emit fatal(tr("Failed to read from device"));
        return;
    }

    const unsigned char check = command + 1;
    const unsigned char ack = buf[0];
    if (ack != check) {
        cout << "device sent unexpected answer, expected = " << check << " received = " << ack << endl;
        disConnect();
        //printf("sent: 0x%02x, received: 0x%02x\n", command, buf[0]);
        emit fatal(QString(tr("Unexpected Answer from device: expected: %1, received:%1")).arg(check).arg(ack));
        return;
    }
}

void Device::ping() {
    comm(DeviceConstants::OP_PING);
}

QString Device::getTdivUnit(DeviceConstants::TdivValues_t val) {
    return (val < DeviceConstants::Tdiv_1 ? "usec/div" : "msec/div");
}

#define ARRSIZE(x) ( sizeof(x)/sizeof(x[0]) )
QString Device::getTdivLabel(DeviceConstants::TdivValues_t val) {
    const char* labels[] = {
        "17.5", "100", "300",
        "1", "3", "10", "30", "100", "300", "1000", "3000"
    };
    Q_ASSERT ( val >= 0 && val < ARRSIZE(labels) );

    return labels[val];
}

double Device::getTdivTime(DeviceConstants::TdivValues_t tdiv, double val) {

    const double mult[] = {
        17.5e-6, 100e-6, 300e-6,
        1e-3, 3e-3, 10e-3, 30e-3, 100e-3, 300e-3, 1000e-3, 3000e-3
    };
    Q_ASSERT ( tdiv >= 0 && tdiv < ARRSIZE(mult) );
    return val * mult[tdiv];
}
QString Device::getVdivLabel(DeviceConstants::VdivValues_t vdiv) {

    const char* labels[] = {
        "10", "5", "2", "1", "0.5", "0.2", "0.1", "0.05"
    };
    Q_ASSERT ( vdiv >= 0 && vdiv < ARRSIZE(labels) );
    return labels[vdiv];
}

double Device::getVdivVoltate(DeviceConstants::VdivValues_t vdiv, double val) {
    const double mult[] = {
        10, 5, 2, 1, 0.5, 0.2, 0.1, 0.05
    };
    Q_ASSERT ( vdiv >= 0 && vdiv < ARRSIZE(mult) );
    return val * mult[vdiv];
}


QString Device::getChannelName(DeviceConstants::Channel_t val) {
    const char* names[] = {
        "Channel 1",
        "Channel 2",
        "Trigger Level"
    };
    Q_ASSERT ( val >= 0 && val < ARRSIZE(names) );
    return names[val];
}

void Device::setVdiv_Ch1(DeviceConstants::VdivValues_t vdiv) {
    config.gainCh1 = gain_ch1_cal[vdiv];
    transmitConfig();
}

void Device::setVdiv_Ch2(DeviceConstants::VdivValues_t vdiv) {
    config.gainCh2 = gain_ch2_cal[vdiv];
    transmitConfig();
}

void Device::setTdiv(DeviceConstants::TdivValues_t div) {
        //"12.8", "30", "100", "300",
        //"1", "3", "10", "30", "100", "300", "1000", "3000"
    unsigned short delay = delay_cal[div];
    //calibDlg.setDelay(delay);
    setDelay(delay);
}

void Device::setDelay(unsigned short val) {
    config.sampleDelay = val;
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

void Device::getADCBlock(QVector<QPointF>& result) {

    DeviceConstants::opcodes_t ops[] = {
        DeviceConstants::OP_SAMPLE_SINGLE,
        DeviceConstants::OP_SAMPLE_DUMMY_TRI,
        DeviceConstants::OP_SAMPLE_DUMMY_ZERO,
        DeviceConstants::OP_SAMPLE_DUMMY_MID
    };

    comm ( ops[dummy] );

    result.clear();
    // comm might have failed and closed the device
    if (!isConnected()) return;



    // 10 divs for 191 samples
    const double timestretch = 10.0/(191.0+64);

    for (size_t i = 1; i < 64; i++ ) {
        result.append(QPointF( timestretch * (i-1), normalizeSample(buf[i])));
    }
    for (int j = 0; j < 3; j++ ) {

        device->read(buf, sizeof(buf)) ;
        for (size_t i = 0; i < 64; i++ ) {
            result.append(QPointF(timestretch*(i + 63 + j*64), normalizeSample(buf[i])));
        }
    }
    cout << "read data from ADC" << endl;
    return;
}

void Device::getADCInterleaved(QVector<QPointF>& ch1, QVector<QPointF>& ch2) {
    comm ( DeviceConstants::OP_SAMPLE_INTERLEAVED);

    // comm might have failed and closed the device
    if (!isConnected()) return;

    // 10 divs for 191 samples
    const double timestretch = 10.0/(191.0+64);

    ch1.clear();
    ch2.clear();

    for (size_t i = 2; i < 64; i++ ) {
        const QPointF point(timestretch * ((i/2) - 1), normalizeSample(buf[i]));
        if (i&1) ch2.append(point);
        else ch1.append(point);
    }
    for (int j = 0; j < 3; j++ ) {

        device->read(buf, sizeof(buf)) ;
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

void Device::selectTriggerMode(DeviceConstants::TriggerMode_t mode) {
    cout << "selecting trigger mode " << mode << endl;
   config.triggerMode = mode;
   transmitConfig();
}

void Device::transmitConfig() {
    cout << "transmitting new config" << endl;
    memcpy(&buf[2], &config, sizeof(DeviceConstants::osci_config_t));
    comm(DeviceConstants::OP_SET_CONFIG);

    // if comm failed, an error has been reported; quit nagging
    if (!isConnected()) return;

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

}
