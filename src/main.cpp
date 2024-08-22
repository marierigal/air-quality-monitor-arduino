#include <Arduino.h>

#include <Leds.h>
Leds leds;

#include <Display.h>
Display display;

#include <EnvSensor.h>
EnvSensor envSensor;

long lastTime = 0;

/**
 * @brief Halt the execution
 */
void halt();

void setup()
{
  /**
   * Initialize the serial port
   */
  Serial.begin(115200);
  long now = millis();
  while (!Serial || millis() - now < 3000)
    delay(10);

  Serial.println();
  Serial.println(F("-------------------------------------"));
  Serial.println(F("|        Air Quality Monitor        |"));
  Serial.println(F("-------------------------------------"));
  Serial.println();

  /**
   * Initialize the LEDs
   */
  if (!leds.begin())
  {
    Serial.println(F("Error initializing LEDs"));
    return halt();
  }

  /**
   * Initialize the display
   */
  if (!display.begin())
  {
    Serial.println(F("Error initializing display"));
    return halt();
  }

  /**
   * Initialize the environment sensor
   */
  EnvSensorStatus envSensorStatus = envSensor.begin();
  if (envSensorStatus != ENV_SENSOR_OK)
  {
    int8_t bme68xStatus = envSensor.getBme68xStatus();
    int8_t bsecStatus = envSensor.getBsecStatus();

    Serial.print(F("Environment sensor status: BME68X "));
    Serial.print(bme68xStatus);
    Serial.print(F(" | BSEC "));
    Serial.println(bsecStatus);

    if (bme68xStatus < BME68X_OK)
    {
      leds.error();
      display.error("BME68X " + String(bme68xStatus));
      return halt();
    }
    else if (bme68xStatus > BME68X_OK)
    {
      leds.warning();
      display.warning("BME68X " + String(bme68xStatus));
      delay(3000);
    }

    if (bsecStatus < BSEC_OK)
    {
      leds.error();
      display.error("BSEC " + String(bsecStatus));
      return halt();
    }
    else if (bsecStatus > BSEC_OK)
    {
      leds.warning();
      display.warning("BSEC " + String(bsecStatus));
      delay(3000);
    }
  }

  Serial.println();
  Serial.print(F("BSEC version: "));
  Serial.println(envSensor.getVersion());
  Serial.println();

  /**
   * Clear the LEDs and the display
   */
  leds.clear();
  display.clear();
}

void loop()
{
  long now = millis();
  if (now - lastTime > 3000)
  {
    Serial.print(F("temperature:"));
    Serial.println(env_sensor::temperature);
    lastTime = now;
  }

  envSensor.update();

  delay(10);
}

void halt()
{
  Serial.println(F("Halting..."));
  while (true)
    delay(10);
}
