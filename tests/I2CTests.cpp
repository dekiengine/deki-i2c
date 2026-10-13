// deki-i2c: the bus registry chip drivers find their bus in, and
// I2CBusComponent, which sets a bus up from the boot scene and registers it.
// The bus is a fake made by the factory a platform package would register.

#include <gtest/gtest.h>

#include "DekiI2C.h"
#include "I2CBusComponent.h"

#include <deki/PackageConfig.h>

#include <string>

using namespace DekiI2c;

namespace
{

struct BusLog
{
    Deki::PackageConfig config;
    int created = 0;
    bool failInitialize = false;
};
BusLog g_Log;

class FakeBus : public IDekiI2C
{
public:
    const char* GetPackageId() const override { return "i2c"; }
    const char* GetPackageName() const override { return "Fake I2C"; }
    void Configure(const Deki::PackageConfig& config) override { g_Log.config = config; }
    bool Initialize() override { return !g_Log.failInitialize; }
    void Shutdown() override {}
    void Update(float) override {}
    Deki::PackageState GetState() const override { return Deki::PackageState::Running; }
    const char* GetLastError() const override { return ""; }
    int GetPort() const override { return 0; }
    int GetSdaPin() const override { return 0; }
    int GetSclPin() const override { return 0; }
    int GetFrequencyHz() const override { return 0; }
    bool Probe(uint8_t) override { return false; }
    bool Read(uint8_t, uint8_t, uint8_t*, size_t) override { return false; }
    bool Write(uint8_t, uint8_t, const uint8_t*, size_t) override { return false; }
};

IDekiI2C* CreateFakeBus()
{
    ++g_Log.created;
    return new FakeBus();
}

class I2CTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        g_Log = BusLog{};
        DekiI2C::SetFactory(&CreateFakeBus);
    }
    void TearDown() override
    {
        DekiI2C::SetFactory(nullptr);
        for (int port = 0; port < DekiI2C::kMaxBuses; ++port)
        {
            delete DekiI2C::GetBus(port);
            DekiI2C::RegisterBus(port, nullptr);
        }
    }

    // Runs Setup and returns what it reported.
    static bool RunSetup(I2CBusComponent& c)
    {
        bool reported = false;
        bool called = false;
        c.Setup(
            [&](bool ok)
            {
                reported = ok;
                called = true;
            });
        EXPECT_TRUE(called);
        return reported;
    }
};

}  // namespace

// --- Registry ---

TEST_F(I2CTest, BusesAreFoundByPort)
{
    FakeBus a;
    FakeBus b;
    DekiI2C::RegisterBus(0, &a);
    DekiI2C::RegisterBus(3, &b);
    EXPECT_EQ(DekiI2C::GetBus(0), &a);
    EXPECT_EQ(DekiI2C::GetBus(3), &b);
    EXPECT_EQ(DekiI2C::GetBus(1), nullptr);
    DekiI2C::RegisterBus(0, nullptr);
    DekiI2C::RegisterBus(3, nullptr);
}

TEST_F(I2CTest, PortsOutsideTheRegistryAreRefused)
{
    FakeBus a;
    DekiI2C::RegisterBus(-1, &a);
    DekiI2C::RegisterBus(DekiI2C::kMaxBuses, &a);
    EXPECT_EQ(DekiI2C::GetBus(-1), nullptr);
    EXPECT_EQ(DekiI2C::GetBus(DekiI2C::kMaxBuses), nullptr);
}

TEST_F(I2CTest, CreateNeedsAFactory)
{
    DekiI2C::SetFactory(nullptr);
    EXPECT_FALSE(DekiI2C::HasFactory());
    EXPECT_EQ(DekiI2C::Create(), nullptr);
    DekiI2C::SetFactory(&CreateFakeBus);
    EXPECT_TRUE(DekiI2C::HasFactory());
    IDekiI2C* bus = DekiI2C::Create();
    EXPECT_NE(bus, nullptr);
    delete bus;
}

// --- I2CBusComponent ---

TEST_F(I2CTest, SetupConfiguresAndRegistersTheBus)
{
    I2CBusComponent c;
    c.i2cPort = 1;
    c.sdaPin = 8;
    c.sclPin = 9;
    c.i2cHz = 100000;
    EXPECT_TRUE(RunSetup(c));
    EXPECT_EQ(g_Log.config.GetPin("SDA"), 8);
    EXPECT_EQ(g_Log.config.GetPin("SCL"), 9);
    EXPECT_EQ(g_Log.config.GetInt("i2cHz"), 100000);
    EXPECT_EQ(g_Log.config.GetInt("i2cPort"), 1);
    EXPECT_NE(DekiI2C::GetBus(1), nullptr);
}

TEST_F(I2CTest, ABusThatWillNotStartIsNotRegistered)
{
    g_Log.failInitialize = true;
    I2CBusComponent c;
    EXPECT_FALSE(RunSetup(c));
    EXPECT_EQ(DekiI2C::GetBus(0), nullptr);
}

TEST_F(I2CTest, NoBackendFailsTheSetup)
{
    DekiI2C::SetFactory(nullptr);
    I2CBusComponent c;
    EXPECT_FALSE(RunSetup(c));
}

TEST_F(I2CTest, APortOutOfRangeFailsTheSetup)
{
    // Registered nowhere, it would leave every chip on it reporting "no bus".
    for (int32_t port : { -1, DekiI2C::kMaxBuses, 7 })
    {
        I2CBusComponent c;
        c.i2cPort = port;
        EXPECT_FALSE(RunSetup(c)) << "port " << port;
    }
    EXPECT_EQ(g_Log.created, 0);
}
