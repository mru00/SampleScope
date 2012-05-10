#include "dummyhardware.h"
#include <cstdlib>
#include <cmath>

DummyHardware::DummyHardware(QObject *parent) :
    AbstractHardware(parent)
  , state(State_Closed)
{
}


void DummyHardware::open() {
    Q_ASSERT(!isOpen());
    state = State_Init;
}

void DummyHardware::close() {
    //Q_ASSERT(isOpen());
    state = State_Closed;
}


static void gen_data(unsigned char* target, int count, unsigned char(*gen)(int i)) {
    for (int i = 0; i < count; i++ ) {
        target[i] = gen(i);
    }
}
static void make_noise(unsigned char* target, int count, int level) {

    for (int i = 0; i < count; i ++ ) {
        target[i] = target[i] + ( (rand()%level) - level/2);
    }
}

static void make_sin(unsigned char* target, int count, int start) {
    for (int i = 0; i < count; i++ ) {
        target[i] = 115*sin( 0.14*(i+start) ) - 127;
    }
}

static void make_sin_2(unsigned char* target, int count, int start) {
    for (int i = 0; i < count; i++ ) {
        target[i] = 126*sin( 0.14*(i+start) ) - 127;
    }
}

static void make_const(unsigned char* target, int count, unsigned char val) {
    for (int i = 0; i < count; i ++ ) {
        target[i] = val;
    }
}

static void make_square(unsigned char* target, int count, unsigned period) {
    for (int i = 0; i < count; i ++ ) {
        target[i] = ( i / period ) & 1 ? 20: 235;
    }
}

int DummyHardware::read(unsigned char *data, size_t length) {

    switch(state) {
    case State_TX_One:
        memcpy(data, buffer, length);
        state = State_Init;
        break;
    case State_Tx_ADC:
        memcpy(data, buffer, length);
        state = State_Tx_ADC_2;
        break;
    case State_Tx_ADC_2:
        memcpy(data, buffer+64, length);
        state = State_Tx_ADC_3;
        break;
    case State_Tx_ADC_3:
        memcpy(data, buffer+128, length);
        state = State_Tx_ADC_4;
        break;
    case State_Tx_ADC_4:
        memcpy(data, buffer+128+64, length);
        state = State_Init;
        break;
    default:
        Q_ASSERT(state == State_TX_One);
    }
    return length;
}

int DummyHardware::get_manufacturer_string(wchar_t *string, size_t maxlen) {
    return mbstowcs(string, "mru", maxlen);
}

int DummyHardware::get_product_string(wchar_t *string, size_t maxlen) {
    return mbstowcs(string, "SampleScope Software Emulation", maxlen);
}


void DummyHardware::makeSignal(DeviceConstants::opcodes_t op) {

    switch (config.inputChannel) {
    case DeviceConstants::ADC_triggerLevel:
        make_const(buffer, 256, 255.0 * config.triggerLevel / 4096.0);
        make_noise(buffer, 256, 10);
        break;

    case DeviceConstants::ADC_ch1:
    case DeviceConstants::ADC_ch2:

        switch(op) {
        case DeviceConstants::OP_SAMPLE_DUMMY_TRI:
            for (int i = 0; i < 256; i ++ ) {
                buffer[i] = i;
            }
            break;
        case DeviceConstants::OP_SAMPLE_DUMMY_ZERO:
            make_const(buffer, 256, 0x00);
            break;
        case DeviceConstants::OP_SAMPLE_DUMMY_MID:
            make_const(buffer, 256, 0x80);
            break;

        case DeviceConstants::OP_SAMPLE_SINGLE:
        case DeviceConstants::OP_SAMPLE_INTERLEAVED:
            if (config.inputChannel == DeviceConstants::ADC_ch1) {
                make_sin(buffer, 256, 0);
                make_noise(buffer, 256, 20);
            }
            else if (config.inputChannel == DeviceConstants::ADC_ch2) {
            //    make_sin(buffer, 256, 90);
                make_square(buffer, 256, 20);
                make_noise(buffer, 256, 20);
            }
            break;
        }
    }
}

int DummyHardware::write(const unsigned char *data, size_t length) {
    Q_ASSERT(state == State_Init);

#define ACK() { buffer[0] = data[1] + 1; }
#define NACK() { buffer[0] = 0xff; }

    DeviceConstants::opcodes_t opcode = (DeviceConstants::opcodes_t) data[1];

    switch(opcode) {

    case DeviceConstants::OP_PING:
        ACK();
        state = State_TX_One;
        break;

    case DeviceConstants::OP_SET_CONFIG:
        ACK();
        buffer[1] = sizeof(DeviceConstants::osci_config_t);
        memcpy(&buffer[2], &data[2], sizeof(DeviceConstants::osci_config_t));
        memcpy(&config, &data[2], sizeof(DeviceConstants::osci_config_t));
        state = State_TX_One;
        break;

    case DeviceConstants::OP_SAMPLE_DUMMY_TRI:
    case DeviceConstants::OP_SAMPLE_DUMMY_ZERO:
    case DeviceConstants::OP_SAMPLE_DUMMY_MID:
    case DeviceConstants::OP_SAMPLE_SINGLE:
    case DeviceConstants::OP_SAMPLE_INTERLEAVED:
        makeSignal(opcode);
        ACK();
        state = State_Tx_ADC;
        break;

    default:
        NACK();
        Q_ASSERT( 1 == 0 );
    }
    return length;
}

bool DummyHardware::isOpen() {
    return state != State_Closed;
}
