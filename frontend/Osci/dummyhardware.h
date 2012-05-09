#ifndef DUMMYHARDWARE_H
#define DUMMYHARDWARE_H

#include "abstracthardware.h"

#include "../../shared/common.h"

class DummyHardware : public AbstractHardware
{

    Q_OBJECT
private:
        enum State_t {
            State_Closed,
            State_Init,
            State_TX_One,
            State_Tx_ADC,
            State_Tx_ADC_2,
            State_Tx_ADC_3,
            State_Tx_ADC_4,
            State_Tx_Last = State_Tx_ADC_4
        };

public:
    explicit DummyHardware(QObject *parent = 0);

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
    unsigned char buffer[4*64];
    DeviceConstants::osci_config_t config;
};

#endif // DUMMYHARDWARE_H
