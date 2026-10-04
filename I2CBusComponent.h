#pragma once

#include <cstdint>
#include <deki/SetupComponent.h>
#include <deki/reflection/Property.h>
#include "IDekiI2C.h"

namespace DekiI2c
{

/// Boot-scene component that starts one physical I2C bus and registers it in
/// DekiI2C, where chip drivers (DS3231RTC and others) find it by port.
///
/// Use one per physical bus; every chip on that bus shares it.
DEKI_CATEGORY("System")
DEKI_DESCRIPTION("Starts one I2C bus at boot. Every chip on that bus shares it.")
DEKI_FORMER_NAME("I2CBusComponent")
class I2CBusComponent : public Deki::SetupComponent
{
public:
    DEKI_EXPORT
    DEKI_TOOLTIP("Which of the chip's I2C controllers to use. Most boards only wire one, so 0 unless your schematic "
                 "says otherwise.")
    DEKI_RANGE(0, 3)
    int32_t i2cPort = 0;

    DEKI_EXPORT
    DEKI_TOOLTIP("GPIO carrying the I2C data line (SDA). Must match how the sensors are wired; every device on this "
                 "bus shares it.")
    DEKI_RANGE(0, 48)
    int32_t sdaPin = 21;

    DEKI_EXPORT
    DEKI_TOOLTIP("GPIO carrying the I2C clock line (SCL). Shared by every device on this bus, like SDA.")
    DEKI_RANGE(0, 48)
    int32_t sclPin = 22;

    DEKI_EXPORT
    DEKI_TOOLTIP("Bus speed. 100 kHz is the safe standard mode, 400 kHz the usual fast mode. Long wires or many "
                 "devices need the lower figure; too high shows up as reads that intermittently fail.")
    DEKI_UNIT(Frequency)
    DEKI_RANGE(10000, 1000000)
    int32_t i2cHz = 400000;

    I2CBusComponent() = default;
    virtual ~I2CBusComponent() = default;

    void Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "I2C Bus"; }

private:
    IDekiI2C* m_Bus = nullptr;
};

}  // namespace DekiI2c
