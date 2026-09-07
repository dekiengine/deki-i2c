#pragma once

#include <cstdint>
#include <deki/SetupComponent.h>
#include <deki/reflection/Property.h>
#include "IDekiI2C.h"

/**
 * @brief Boot-prefab component that initializes one physical I2C bus and
 *        publishes it in DekiI2C so chip drivers (DS3231RTC, etc.)
 *        can look it up by port number.
 *
 * One instance per physical bus. Multiple chips on the same bus share this
 * single component.
 */
DEKI_CATEGORY("System")
DEKI_DESCRIPTION("Starts one I2C bus at boot. Every chip on that bus shares it.")
class I2CBusComponent : public Deki::SetupComponent
{
public:

    DEKI_EXPORT
    DEKI_RANGE(0, 3)
    int32_t i2cPort = 0;

    DEKI_EXPORT
    DEKI_RANGE(0, 48)
    int32_t sdaPin = 21;

    DEKI_EXPORT
    DEKI_RANGE(0, 48)
    int32_t sclPin = 22;

    DEKI_EXPORT
    DEKI_RANGE(10000, 1000000)
    int32_t i2cHz = 400000;

    I2CBusComponent() = default;
    virtual ~I2CBusComponent() = default;

    void        Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "I2C Bus"; }

private:
    IDekiI2C* m_Bus = nullptr;
};

