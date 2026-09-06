#pragma once

#include "IDekiI2C.h"
#include "DekiI2CPackage.h"

/**
 * @brief Provider/registry for I2C buses.
 *
 * - Platform package registers a factory via SetFactory().
 * - I2CBusComponent (one per physical bus in boot.prefab) calls Create() then
 *   Configure()/Initialize(), and finally RegisterBus(port, bus) so chip
 *   drivers can look it up by port number.
 * - Chip drivers (DS3231RTC, etc.) call GetBus(port) once during Initialize().
 */
class DEKI_I2C_API DekiI2C
{
public:
    using Factory = IDekiI2C* (*)();

    static void      SetFactory(Factory factory);
    static IDekiI2C* Create();
    static bool      HasFactory();

    static void      RegisterBus(int port, IDekiI2C* bus);
    static IDekiI2C* GetBus(int port);

    static constexpr int kMaxBuses = 4;

private:
    static Factory   s_Factory;
    static IDekiI2C* s_Buses[kMaxBuses];
};
