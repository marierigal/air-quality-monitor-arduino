#include <Arduino.h>

#include <Leds.h>
Leds leds;

#include <Display.h>
Display display;

#include <EnvSensor.h>
EnvSensor envSensor;

long lastTime = 0;

void setup()
{
  Serial.begin(115200);
  long now = millis();
  while (!Serial || millis() - now < 3000)
    delay(10);

  Serial.println();
  Serial.println(F("-------------------------------------"));
  Serial.println(F("|        Air Quality Monitor        |"));
  Serial.println(F("-------------------------------------"));
  Serial.println();

  leds.begin();
  display.begin();
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
