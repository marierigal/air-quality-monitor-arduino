# Air Quality Monitor

This Arduino project read data from BM68x sensor and use BSEC library to get IAQ (Indoor Air Quality) data.

## Hardware

- [Arduino MKR WiFi 1010](https://docs.arduino.cc/hardware/mkr-wifi-1010)
- [Arduino MKR IoT Carrier (rev.1)](https://docs.arduino.cc/hardware/mkr-iot-carrier)
- [BME688](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme688/)

## Software

- [Bosch BME68x library](https://github.com/boschsensortec/Bosch-BME68x-Library) (for sensor data)
- [Bosch BSEC2 library](https://github.com/boschsensortec/Bosch-BSEC2-Library) (for IAQ calculation)
- [Adafruit DotStar](https://github.com/adafruit/Adafruit_DotStar) (for RGB LEDs)
- [Adafruit ST7735 and ST7789 Library](https://github.com/adafruit/Adafruit-ST7735-Library) (for TFT display)
- [SD](https://github.com/arduino-libraries/SD) (for SD card)

> Please note that you can include the [Arduino_MKRIoTCarrier](https://github.com/arduino-libraries/Arduino_MKRIoTCarrier) library to have a unified object for accessing the peripherals of the carrier.

## Wiring

### MKR IoT Carrier

Connect the MKR WiFi 1010 to the MKR IoT Carrier.

### BME688

Connect the BME688 sensor to the Arduino board using the I2C interface.

| BME688      | I2C      |
| ----------- | -------- |
| VCC         | 3.3V     |
| GND         | GND      |
| SCL / SCK   | A5 (SCL) |
| ADDR / MISO | GND      |
| SDA / MOSI  | A4 (SDA) |
| CS          | -        |

> Please note that the `ADDR` pin is used to select the I2C address of the sensor. If `ADDR` is connected to `GND`, the address is `0x76`, `0x77` otherwise.

## Usage

This project is configured to work with [PlatformIO](https://platformio.org). You can use the `platformio.ini` file to configure the project settings.

1. Open the project in PlatformIO.
2. Install the required libraries.
3. Build and upload the project to the Arduino board.

> Please note that you need to configure the `bsec_iaq.config` file with the correct sensor settings. You can use the [BSEC integration guide](doc/BST-BME-Integration-Guide-AN011-50.pdf) to generate the configuration file.

## Useful links

- [Arduino documentation](https://docs.arduino.cc)
- [Adafruit documentation](https://learn.adafruit.com)
- [Bosch BME688 product page](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme688/)
- [BSEC integration guide](doc/BST-BME-Integration-Guide-AN011-50.pdf)
- [Waveshare BME68X documentation](https://www.waveshare.com/wiki/BME680_Environmental_Sensor)
- [Image to C++ converter](https://javl.github.io/image2cpp)

## TODO

- [x] Save and load sensor configuration on SD card
- [ ] Display IAQ data on TFT display
- [ ] Use touch buttons to interact with the display
- [ ] Use relay to control air purifier
