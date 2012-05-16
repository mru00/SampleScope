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


static void make_noise(unsigned char* target, int count, int level) {

    for (int i = 0; i < count; i ++ ) {
        target[i] = target[i] + ( (rand()%level) - level/2);
    }
}

static void make_sin(unsigned char* target, int count, int phase, double stretch) {
    for (int i = 0; i < count; i++ ) {
        target[i] = 115*sin( (0.1+stretch)*0.0001*(i+phase) ) - 127;
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
        memcpy(data, buffer + (SAMPLE_BUFFER_SIZE * bufferTransmitCounter), length);
        if (++bufferTransmitCounter == SAMPLE_BUFFER_COUNT)
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


void DummyHardware::makeSignal(DeviceConstants::Channel_t ch, DeviceConstants::TestSignal_t ts) {

    switch(ts) {
    case DeviceConstants::TestSignal_Off:
        if (ch == DeviceConstants::ADC_ch1) {
            make_sin(buffer, SAMPLE_COUNT, 0, config.sampleDelay);
            make_noise(buffer, SAMPLE_COUNT, 20);
        }
        else if (ch == DeviceConstants::ADC_ch2) {
            //    make_sin(buffer, 256, 90);
            make_square(buffer, SAMPLE_COUNT, 20);
            make_noise(buffer, SAMPLE_COUNT, 20);
        }
        break;
    case DeviceConstants::TestSignal_Tri:
        for (int i = 0; i < SAMPLE_COUNT; i ++ ) {
            buffer[i] = i;
        }
        break;
    case DeviceConstants::TestSignal_Tri_2:
        for (int j = 0; j < SAMPLE_BUFFER_COUNT; j++ ) {
            for (int i = 0; i < SAMPLE_BUFFER_SIZE; i ++ ) {
                buffer[j*SAMPLE_BUFFER_SIZE + i] = i;
            }
        }
        break;
    case DeviceConstants::TestSignal_Mid:
        make_const(buffer, SAMPLE_COUNT, 0x80);
        break;

    case DeviceConstants::TestSignal_Zero:
        make_const(buffer, SAMPLE_COUNT, 0x00);
        break;
    }

}

int DummyHardware::write(const unsigned char *data, size_t length) {
    Q_ASSERT(state == State_Init);

#define ACK() { encoder->ack = data[1] + 1; }
#define NACK() { encoder->ack = 0xff; }

    DeviceConstants::opcode_decoder_t* decoder = (DeviceConstants::opcode_decoder_t*)&data[1];
    DeviceConstants::result_encoder_t* encoder = (DeviceConstants::result_encoder_t*)&buffer[0];

    switch(decoder->opcode) {

    case DeviceConstants::OP_PING:
        ACK();
        state = State_TX_One;
        break;

    case DeviceConstants::OP_SET_CONFIG:
        ACK();
        config = decoder->args.set_config_args;
        state = State_TX_One;
        break;

    case DeviceConstants::OP_GET_INFO:
    {
        DeviceConstants::device_info_t info;
        info.sizeInfo = sizeof(DeviceConstants::device_info_t);
        info.sizeConfig = sizeof(DeviceConstants::osci_config_t);
        info.bufferCount = SAMPLE_BUFFER_COUNT;
        info.bufferSize = SAMPLE_BUFFER_SIZE;
        info.version = 0;
        ACK();
        encoder->args.get_info_args = info;
        state = State_TX_One;
    }
    break;

    case DeviceConstants::OP_SAMPLE_SINGLE:
        switch (decoder->args.sample_single_args.inputChannel) {
        case DeviceConstants::ADC_triggerLevel:
            make_const(buffer, SAMPLE_COUNT, 255.0 * config.triggerLevel / 4096.0);
            make_noise(buffer, SAMPLE_COUNT, 10);
            break;

        case DeviceConstants::ADC_ch1:
        case DeviceConstants::ADC_ch2:

            makeSignal(decoder->args.sample_single_args.inputChannel, decoder->args.sample_single_args.testSignal);
            break;
        }
        ACK();
        bufferTransmitCounter = 0;
        state = State_Tx_ADC;

        break;
    case DeviceConstants::OP_SAMPLE_INTERLEAVED:
        makeSignal(DeviceConstants::ADC_ch1, decoder->args.sample_single_args.testSignal);
        ACK();
        bufferTransmitCounter = 0;
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
