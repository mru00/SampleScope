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


#ifndef ABSTRACTHARDWARE_H
#define ABSTRACTHARDWARE_H

#include <QObject>

/**
 * AbstractHardware describes the interface to the physical device.
 *
 * The class exacly models the api of HIDAPI.
 *
 * Two subclasses exists: RealHardware and DummyHardware. RealHardware just forwards all
 * the calls to HIDAPI, trying to talk to the physical device via USB.
 *
 * DummyHardware implements the same behaviour as the physical device, abstracted by the
 * write/read interface. It allows testing of the GUI without the actual hardware and acts
 * as an simple signal generator.
 */
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

    virtual Impl_t getImpl() = 0;

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
