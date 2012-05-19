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


#include "abstracthardware.h"

#include "realhardware.h"
#include "dummyhardware.h"

AbstractHardware::AbstractHardware(QObject *parent) :
    QObject(parent)
{
}


AbstractHardware* AbstractHardware::create(AbstractHardware::Impl_t impl) {
    switch (impl) {
    case AbstractHardware::Impl_Dummy: return new DummyHardware();
    case AbstractHardware::Impl_Real: return new RealHardware();
    }
    return NULL;
}

