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


#include "realhardware.h"
#include "../../shared/common.h"

RealHardware::RealHardware(QObject *parent) :
    AbstractHardware(parent),
    device(NULL)
{
}

void RealHardware::open() {
    device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
}

void RealHardware::close() {
    hid_close(device);
    device = NULL;
}

int RealHardware::read(unsigned char *data, size_t length) {
    return hid_read_timeout(device, data, length, 10000);
}

int RealHardware::get_manufacturer_string(wchar_t *string, size_t maxlen) {
    if(!isOpen()) return -1;
    return hid_get_manufacturer_string(device, string, maxlen);
}

int RealHardware::get_product_string(wchar_t *string, size_t maxlen) {
    return hid_get_product_string(device, string, maxlen);
}

int RealHardware::write(const unsigned char *data, size_t length) {
    return hid_write(device, data, length);
}

bool RealHardware::isOpen() {
    return device != NULL;
}
