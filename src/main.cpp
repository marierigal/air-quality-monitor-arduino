#include <Arduino.h>

#include <Leds.h>
#include <Display.h>
#include <Data.h>
#include <EnvSensor.h>

#define BSEC_SAVE_STATE 1
#define BSEC_STATE_SAVE_INTERVAL (5 * 60 * 1000)

Leds leds;
Display display;
Data data;
EnvSensor envSensor;
unsigned long lastBsecStateSave;

/**
 * @brief Halt the execution
 */
void halt();

/**
 * @brief Get BSEC state from SD card
 */
bool loadBsecState(uint8_t *state);

/**
 * @brief Save BSEC state to SD card
 */
bool saveBsecState();

/**
 * @brief Get BSEC state directory
 */
String getBsecStateFileDir();

/**
 * @brief Get BSEC state filename
 */
String getBsecStateFilename();

/**
 * @brief Setup
 */
void setup()
{
  /**
   * Initialize the serial port
   */
  Serial.begin(115200);
  long now = millis();
  while (!Serial && millis() - now < 3000)
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
   * Initialize the SD card
   */
  if (!data.begin())
  {
    Serial.println(F("Error initializing SD card"));
    return halt();
  }

  /**
   * Initialize the environment sensor
   */
  EnvSensorStatus envSensorStatus = envSensor.begin(loadBsecState);
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

/**
 * @brief Loop
 */
void loop()
{
#if BSEC_SAVE_STATE
  long now = millis();
  if (now - lastBsecStateSave > BSEC_STATE_SAVE_INTERVAL)
  {
    Serial.print(F("temperature:"));
    Serial.println(env_sensor::temperature);
    Serial.print(F("humidity:"));
    Serial.println(env_sensor::humidity);
    Serial.print(F("iaq:"));
    Serial.println(env_sensor::iaq);
    Serial.print(F("accuracy:"));
    Serial.println(env_sensor::iaqAccuracy);

    if (saveBsecState())
      Serial.println(F("BSEC state saved"));
    else
      Serial.println(F("Error saving BSEC state"));

    Serial.println();

    lastBsecStateSave = now;
  }
#endif

  envSensor.update();

  delay(10);
}

void halt()
{
  Serial.println(F("Halting..."));
  while (true)
    delay(10);
}

bool loadBsecState(uint8_t *state)
{
  return data.readFile(getBsecStateFilename(), state, EnvSensor::STATE_MAX_SIZE);
}

bool saveBsecState()
{
  uint8_t n_state = EnvSensor::STATE_MAX_SIZE;
  uint8_t state[n_state];

  if (!envSensor.getState(state))
    return false;

  String filename = getBsecStateFilename();

  if (!data.existsFile(filename))
    data.createDir(getBsecStateFileDir());

  return data.writeFile(filename, state, n_state, 0);
}

String getBsecStateFileDir()
{
  String version = envSensor.getVersion();
  version.replace(".", "-");
  return "bsec/v" + version;
}

String getBsecStateFilename()
{
  return getBsecStateFileDir() + "/state.txt";
}
