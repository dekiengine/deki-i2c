#pragma once

#include "IDekiI2C.h"
#include "DekiI2CPackage.h"

namespace DekiI2c
{

/// Creates I2C buses and finds them by port.
///
/// - The platform package registers a factory with SetFactory().
/// - I2CBusComponent (one per physical bus in the boot scene) calls Create(),
///   then Configure() and Initialize(), then RegisterBus(port, bus).
/// - Chip drivers (DS3231RTC and others) call GetBus(port) once in
///   Initialize().
class DEKI_I2C_API DekiI2C
{
public:
    using Factory = IDekiI2C* (*)();

    static void SetFactory(Factory factory);
    static IDekiI2C* Create();
    static bool HasFactory();

    static void RegisterBus(int port, IDekiI2C* bus);
    static IDekiI2C* GetBus(int port);

    static constexpr int kMaxBuses = 4;

private:
    static Factory s_Factory;
    static IDekiI2C* s_Buses[kMaxBuses];
};

}  // namespace DekiI2c
