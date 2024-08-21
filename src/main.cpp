#include <Arduino.h>

#define DEBUG 1

#include <Leds.h>
Leds leds;

#include <EnvSensor.h>
EnvSensor envSensor;

long lastTime = 0;

void setup()
{
  Serial.begin(115200);
#if (DEBUG)
  while (!Serial)
    delay(10);
#endif

  Serial.println();
  Serial.println(F("-------------------------------------"));
  Serial.println(F("|        Air Quality Monitor        |"));
  Serial.println(F("-------------------------------------"));
  Serial.println();

  leds.begin();
  envSensor.begin();
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
