/**
 * @file DekiI2CPackage.cpp
 * @brief Package entry point for deki-i2c
 */
#include "DekiI2CPackage.h"
#include <deki/interop/Plugin.h>
#include <deki/LogSystem.h>

#ifdef DEKI_EDITOR
extern void DekiI2C_RegisterComponents();
extern int  DekiI2C_GetAutoComponentCount();
extern const Deki::ComponentMeta* DekiI2C_GetAutoComponentMeta(int index);
#endif

static bool s_I2CRegistered = false;

extern "C" {

DEKI_I2C_API int DekiI2C_EnsureRegistered(void)
{
#ifdef DEKI_EDITOR
    if (s_I2CRegistered) return DekiI2C_GetAutoComponentCount();
    s_I2CRegistered = true;
    DekiI2C_RegisterComponents();
    return DekiI2C_GetAutoComponentCount();
#else
    return 0;
#endif
}

DEKI_PLUGIN_API const char* DekiPlugin_GetName(void)    { return "Deki I2C Package"; }
DEKI_PLUGIN_API const char* DekiPlugin_GetVersion(void)
{
#ifdef DEKI_PACKAGE_VERSION
    return DEKI_PACKAGE_VERSION;
#else
    return "0.0.0-dev";
#endif
}
DEKI_PLUGIN_API int  DekiPlugin_Init(void)     { DEKI_LOG_INFO("[deki-i2c] DekiPlugin_Init"); return 0; }
DEKI_PLUGIN_API void DekiPlugin_Shutdown(void) { s_I2CRegistered = false; }

#ifdef DEKI_EDITOR
DEKI_PLUGIN_API int  DekiPlugin_GetComponentCount(void) { return DekiI2C_GetAutoComponentCount(); }
DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPlugin_GetComponentMeta(int index)
{
    return DekiI2C_GetAutoComponentMeta(index);
}
#else
DEKI_PLUGIN_API int  DekiPlugin_GetComponentCount(void) { return 0; }
DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPlugin_GetComponentMeta(int) { return nullptr; }
#endif

DEKI_PLUGIN_API void DekiPlugin_RegisterComponents(void)
{
#ifdef DEKI_EDITOR
    int n = DekiI2C_EnsureRegistered();
    DEKI_LOG_INFO("[deki-i2c] DekiPlugin_RegisterComponents -> %d component(s)", n);
#endif
}


}  // extern "C"
