#include <Arduino.h>

#define DEBUG 1

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
  Serial.println("-------------------------------------");
  Serial.println("|        Air Quality Monitor        |");
  Serial.println("-------------------------------------");
  Serial.println();

  envSensor.begin();
}

void loop()
{
  long now = millis();
  if (now - lastTime > 3000)
  {
    Serial.println("temperature:" + String(env_sensor::temperature));
    lastTime = now;
  }

  envSensor.update();

  delay(10);
}
