#include "I2CBusComponent.h"
#include "DekiI2C.h"
#include <deki/PackageConfig.h>
#include <deki/LogSystem.h>
#include <string>

namespace DekiI2c
{

void I2CBusComponent::Setup(SetupCallback onComplete)
{
    // A port the registry cannot hold would set the bus up and register
    // nothing, and every chip on it would then fail with "no I2C bus".
    if (i2cPort < 0 || i2cPort >= DekiI2C::kMaxBuses)
    {
        DEKI_LOG_ERROR("I2CBusComponent: I2C port %d is out of range (0-%d)", (int)i2cPort, DekiI2C::kMaxBuses - 1);
        if (onComplete)
        {
            onComplete(false);
        }
        return;
    }

    if (!m_Bus)
    {
        m_Bus = DekiI2C::Create();
        if (!m_Bus)
        {
            DEKI_LOG_ERROR("I2CBusComponent: No I2C backend registered");
            if (onComplete)
            {
                onComplete(false);
            }
            return;
        }
    }

    Deki::PackageConfig config;
    config.packageId = "i2c";
    config.enabled = true;
    config.pins["SDA"] = sdaPin;
    config.pins["SCL"] = sclPin;
    config.settings["i2cHz"] = std::to_string(i2cHz);
    config.settings["i2cPort"] = std::to_string(i2cPort);

    m_Bus->Configure(config);

    bool success = m_Bus->Initialize();
    if (success)
    {
        DekiI2C::RegisterBus(i2cPort, m_Bus);
    }
    else
    {
        DEKI_LOG_ERROR("I2CBusComponent: Failed to initialize I2C bus on port %d", (int)i2cPort);
    }

    if (onComplete)
    {
        onComplete(success);
    }
}

}  // namespace DekiI2c
