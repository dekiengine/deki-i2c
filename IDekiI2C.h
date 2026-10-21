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

    virtual int GetPort() const = 0;
    virtual int GetSdaPin() const = 0;
    virtual int GetSclPin() const = 0;
    virtual int GetFrequencyHz() const = 0;

    virtual bool Probe(uint8_t addr) = 0;
    virtual bool Read(uint8_t addr, uint8_t reg, uint8_t* dst, size_t len) = 0;
    virtual bool Write(uint8_t addr, uint8_t reg, const uint8_t* src, size_t len) = 0;

    /// A read with no register sent first, for devices that are not register
    /// maps: an I2C keyboard answers any read with the next character and
    /// takes a written byte as a command, so Read() would change its
    /// settings. Not pure virtual, so a backend without it still compiles; it
    /// just cannot talk to such a device.
    virtual bool ReadRaw(uint8_t addr, uint8_t* dst, size_t len)
    {
        (void)addr;
        (void)dst;
        (void)len;
        return false;
    }

    /// Writes `tx`, then reads `rxLen` bytes, as one transaction with a
    /// repeated start: for a device whose registers are not one byte wide (a
    /// GT911 touch controller's are 16-bit, high byte first). Not pure
    /// virtual, like ReadRaw.
    virtual bool WriteRead(uint8_t addr, const uint8_t* tx, size_t txLen, uint8_t* rx, size_t rxLen)
    {
        (void)addr;
        (void)tx;
        (void)txLen;
        (void)rx;
        (void)rxLen;
        return false;
    }

    /// Writes `tx` as it is, with no register byte in front: the register is
    /// in `tx` when it is not one byte wide. Not pure virtual, like ReadRaw.
    virtual bool WriteRaw(uint8_t addr, const uint8_t* tx, size_t len)
    {
        (void)addr;
        (void)tx;
        (void)len;
        return false;
    }
};

}  // namespace DekiI2c
