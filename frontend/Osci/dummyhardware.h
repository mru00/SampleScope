/*

SampleScope: A humble USB oscilloscope
Copyright (C) 2012 mru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
            SAMPLE_BUFFER_COUNT  = 8,
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
