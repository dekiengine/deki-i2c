# Deki I2C

Documentation: https://dekiengine.github.io/deki-i2c/ (components and properties, generated from the code)

I2C bus peripheral interface for the Deki Engine. Defines `IDekiI2C` (probe, register read, register write, bus configuration) plus an `I2CBusComponent` for configuring a bus from the editor.

Peripheral packages such as `deki-rtc` and `deki-imu` talk to their devices through this interface rather than to a platform driver directly.

Part of the [Deki Engine](https://github.com/dekiengine/deki-engine) package ecosystem.

## Installation

Install via the Package Manager inside the Deki Editor.

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.
