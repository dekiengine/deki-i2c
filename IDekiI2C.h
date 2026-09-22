#pragma once

#include <deki/providers/IPackage.h>
#include <cstdint>
#include <cstddef>

namespace DekiI2c
{

class IDekiI2C : public Deki::IPackage
{
public:
    const char* GetPackageCategory() const override { return "i2c"; }

    virtual int  GetPort() const = 0;
    virtual int  GetSdaPin() const = 0;
    virtual int  GetSclPin() const = 0;
    virtual int  GetFrequencyHz() const = 0;

    virtual bool Probe(uint8_t addr) = 0;
    virtual bool Read (uint8_t addr, uint8_t reg, uint8_t* dst, size_t len) = 0;
    virtual bool Write(uint8_t addr, uint8_t reg, const uint8_t* src, size_t len) = 0;

    // A read with no register sent first. Some devices are not register maps:
    // an I2C keyboard answers any read with the next character, and treats a
    // byte written to it as a command, so Read() above would change its
    // settings. Not pure, so a backend written before this still compiles; it
    // just cannot talk to such a device.
    virtual bool ReadRaw(uint8_t addr, uint8_t* dst, size_t len)
    {
        (void)addr; (void)dst; (void)len;
        return false;
    }
};

}  // namespace DekiI2c
