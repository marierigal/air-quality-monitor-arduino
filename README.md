# BME68x & BSEC

This Arduino project read data from BM68x sensor and use BSEC library to get IAQ (Indoor Air Quality) data.

## Hardware

- [BME688](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme688/)
- [Arduino Nano 33 IoT](https://store.arduino.cc/arduino-nano-33-iot)

## Software

- [Bosch BME68x library](https://github.com/boschsensortec/Bosch-BME68x-Library)
- [Bosch BSEC2 library](https://github.com/boschsensortec/Bosch-BSEC2-Library)

## Wiring

| BME688      | SPI        | I2C      |
| ----------- | ---------- | -------- |
| VCC         | 3.3V       | 3.3V     |
| GND         | GND        | GND      |
| SCL / SCK   | D13 (SCK)  | A5 (SCL) |
| ADDR / MISO | D12 (MISO) | -        |
| SDA / MOSI  | D11 (MOSI) | A4 (SDA) |
| CS          | D10        | -        |

> Note: `ADDR` pin is used to select the I2C address of the sensor. If `ADDR` is connected to `GND`, the address is `0x76`, `0x77` otherwise.

## References

- [Bosch BME688 product page](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme688/)
- [BSEC integration guide](doc/BST-BME-Integration-Guide-AN011-50.pdf)
- [Waveshare BME68X documentation](https://www.waveshare.com/wiki/BME680_Environmental_Sensor)
