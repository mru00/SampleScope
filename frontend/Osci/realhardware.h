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


#ifndef REALHARDWARE_H
#define REALHARDWARE_H

#include "abstracthardware.h"
#include "hidapi.h"


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
