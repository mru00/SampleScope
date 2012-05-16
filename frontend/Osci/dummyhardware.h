#ifndef DUMMYHARDWARE_H
#define DUMMYHARDWARE_H

#include "abstracthardware.h"

#include "../../shared/common.h"

/** this class implements large parts of the read hardware and emulates it's behaviour */
class DummyHardware : public AbstractHardware
{

    Q_OBJECT
private:
        enum State_t {
            State_Closed,
            State_Init,
            State_TX_One,
            State_Tx_ADC,
            State_Tx_Last = State_Tx_ADC
        };

        enum {
            SAMPLE_BUFFER_COUNT  = 4,
            SAMPLE_BUFFER_SIZE  = 64,
            SAMPLE_COUNT = (SAMPLE_BUFFER_COUNT*SAMPLE_BUFFER_SIZE)
        };

public:
    explicit DummyHardware(QObject *parent = 0);

    virtual Impl_t getImpl() { return AbstractHardware::Impl_Dummy; }

    virtual void open();
    virtual void close();
    virtual int read(unsigned char *data, size_t length);
    virtual int get_manufacturer_string(wchar_t *string, size_t maxlen);
    virtual int get_product_string(wchar_t *string, size_t maxlen);
    virtual int write(const unsigned char *data, size_t length);
    virtual bool isOpen();
signals:

public slots:

private:
    State_t state;
    unsigned char buffer[SAMPLE_COUNT];
    DeviceConstants::osci_config_t config;

    int bufferTransmitCounter;

    void makeSignal(DeviceConstants::Channel_t, DeviceConstants::TestSignal_t);
};

#endif // DUMMYHARDWARE_H
