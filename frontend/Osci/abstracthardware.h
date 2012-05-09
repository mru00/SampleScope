#ifndef ABSTRACTHARDWARE_H
#define ABSTRACTHARDWARE_H

#include <QObject>

class AbstractHardware : public QObject
{
    Q_OBJECT

public:
    enum Impl_t {
        Impl_Real,
        Impl_Dummy
    };

protected:

    explicit AbstractHardware(QObject *parent = 0);

public:
    virtual ~AbstractHardware() {}

public:

    static AbstractHardware* create(AbstractHardware::Impl_t);

    virtual void open() = 0;
    virtual void close() = 0;
    virtual int read(unsigned char *data, size_t length) = 0;
    virtual int get_manufacturer_string(wchar_t *string, size_t maxlen) = 0;
    virtual int get_product_string(wchar_t *string, size_t maxlen) = 0;
    virtual int write(const unsigned char *data, size_t length) = 0;
    virtual bool isOpen() = 0;

signals:

public slots:

};

#endif // ABSTRACTHARDWARE_H
