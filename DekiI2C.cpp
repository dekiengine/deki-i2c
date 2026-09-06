#include "DekiI2C.h"
#include <deki/LogSystem.h>

DekiI2C::Factory DekiI2C::s_Factory = nullptr;
IDekiI2C*        DekiI2C::s_Buses[DekiI2C::kMaxBuses] = {};

void DekiI2C::SetFactory(Factory factory)
{
    s_Factory = factory;
    DEKI_LOG_INTERNAL("DekiI2C: Factory registered");
}

IDekiI2C* DekiI2C::Create()
{
    if (!s_Factory)
    {
        DEKI_LOG_ERROR("DekiI2C: No factory registered - platform package must call SetFactory()");
        return nullptr;
    }
    return s_Factory();
}

bool DekiI2C::HasFactory()
{
    return s_Factory != nullptr;
}

void DekiI2C::RegisterBus(int port, IDekiI2C* bus)
{
    if (port < 0 || port >= kMaxBuses)
    {
        DEKI_LOG_ERROR("DekiI2C::RegisterBus: port %d out of range [0,%d)", port, kMaxBuses);
        return;
    }
    s_Buses[port] = bus;
}

IDekiI2C* DekiI2C::GetBus(int port)
{
    if (port < 0 || port >= kMaxBuses) return nullptr;
    return s_Buses[port];
}
