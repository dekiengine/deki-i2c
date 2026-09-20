# Deki I2C

Docs: https://dekiengine.github.io/deki-i2c/ (components and properties, generated from the code)

I2C bus peripheral interface for the Deki Engine. Defines `IDekiI2C` (probe, register read, register write, bus configuration) plus an `I2CBusComponent` for configuring a bus from the editor.

Peripheral packages such as `deki-rtc` and `deki-imu` talk to their devices through this interface rather than to a platform driver directly.

Part of [Deki Engine](https://github.com/dekiengine/deki-engine).

## Namespace

Types live in `DekiI2c`. Scene files store the qualified name, and so does code:

```cpp
using namespace DekiI2c;
obj->AddComponent<SomeComponent>();
```

Scenes saved before 0.16.0 used bare names and still load; saving writes the current one.

## Install

Package Manager in the Deki Editor, or `DekiEditor --packages-add deki-i2c <project>`.

## License

Apache 2.0. See [LICENSE](LICENSE).
