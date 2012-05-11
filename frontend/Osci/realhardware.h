#ifndef REALHARDWARE_H
#define REALHARDWARE_H

#include "abstracthardware.h"
#include "../HIDAPI/hidapi.h"


/* Implements the interface to HIDAPI, therefore to the real device */
class RealHardware : public AbstractHardware
{
    Q_OBJECT
public:
    explicit RealHardware(QObject *parent = 0);

    virtual Impl_t getImpl() { return AbstractHardware::Impl_Real; }

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
    hid_device* device;
};

#endif // REALHARDWARE_H
