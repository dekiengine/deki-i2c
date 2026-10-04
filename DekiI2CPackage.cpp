// Package entry point for deki-i2c.
#include "DekiI2CPackage.h"
#include <deki/interop/Plugin.h>
#include <deki/LogSystem.h>

extern void DekiI2CRegisterComponents();
extern int DekiI2CGetAutoComponentCount();
extern const Deki::ComponentMeta* DekiI2CGetAutoComponentMeta(int index);

namespace DekiI2c
{

#ifdef DEKI_EDITOR
#endif

static bool s_I2CRegistered = false;

}  // namespace DekiI2c
// The exports below are C symbols at global scope; the package's own
// registration helpers and statics live in its namespace.
using namespace DekiI2c;

extern "C"
{
    DEKI_I2C_API int DekiI2CEnsureRegistered(void)
    {
#ifdef DEKI_EDITOR
        if (s_I2CRegistered)
        {
            return ::DekiI2CGetAutoComponentCount();
        }
        s_I2CRegistered = true;
        ::DekiI2CRegisterComponents();
        return ::DekiI2CGetAutoComponentCount();
#else
        return 0;
#endif
    }

    DEKI_PLUGIN_API const char* DekiPluginGetName(void)
    {
        return "Deki I2C Package";
    }
    DEKI_PLUGIN_API const char* DekiPluginGetVersion(void)
    {
#ifdef DEKI_PACKAGE_VERSION
        return DEKI_PACKAGE_VERSION;
#else
        return "0.0.0-dev";
#endif
    }
    DEKI_PLUGIN_API int DekiPluginInit(void)
    {
        return 0;
    }
    DEKI_PLUGIN_API void DekiPluginShutdown(void)
    {
        s_I2CRegistered = false;
    }

#ifdef DEKI_EDITOR
    DEKI_PLUGIN_API int DekiPluginGetComponentCount(void)
    {
        return ::DekiI2CGetAutoComponentCount();
    }
    DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPluginGetComponentMeta(int index)
    {
        return ::DekiI2CGetAutoComponentMeta(index);
    }
#else
    DEKI_PLUGIN_API int DekiPluginGetComponentCount(void)
    {
        return 0;
    }
    DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPluginGetComponentMeta(int)
    {
        return nullptr;
    }
#endif

    DEKI_PLUGIN_API void DekiPluginRegisterComponents(void)
    {
#ifdef DEKI_EDITOR
        DekiI2CEnsureRegistered();
#endif
    }

}  // extern "C"
