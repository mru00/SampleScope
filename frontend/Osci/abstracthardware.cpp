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

