#include <Arduino.h>

#include <Leds.h>
#include <Display.h>
#include <Qtouch.h>
#include <Data.h>
#include <EnvSensor.h>

#include "bitmap_droplet.h"
#include "bitmap_leaf.h"
#include "bitmap_temperature.h"

#define TOUCH_SENSITIVITY 100 // Set to 200 if using the case, 100 otherwise

#define BSEC_SAVE_STATE 1
#define BSEC_STATE_SAVE_INTERVAL (5 * 60 * 1000)

#define STATE_TIMEOUT (10 * 1000)

typedef enum
{
  APP_STATE_IDLE,
  APP_STATE_TEMPERATURE,
  APP_STATE_HUMIDITY,
  APP_STATE_IAQ,
} AppState;

AppState appState = APP_STATE_IDLE;
AppState lastAppState = appState;
long lastAppStateChange;
bool appStateAuto = true;
Leds leds;
Display display;
Qtouch qtouch;
Data data;
EnvSensor envSensor;
String lastData;
String lastAccuracy;
long lastBsecStateSave;

/**
 * @brief Halt the execution
 */
void halt();

/**
 * @brief Set the application state
 */
void setAppState(AppState state, bool stateAuto = true);

/**
 * @brief Switch to next application state
 */
void nextAppState();

/**
 * @brief Display a data screen
 * @param background The background color
 * @param icon The icon to display
 * @param data The data to display
 */
void displayDataScreen(uint16_t background, const byte *icon, String data, String accuracy = "");

/**
 * @brief Flash a LED
 * @param index The LED index
 */
void flashLed(uint8_t index);

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
 * @brief Handle the environment sensor status
 * @param status The environment sensor status
 */
void handleEnvSensorStatus(EnvSensorStatus status, bool showWarning);

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
   * Initialize the touch sensor
   */
  if (!qtouch.begin(TOUCH_SENSITIVITY))
  {
    Serial.println(F("Error initializing touch sensor"));
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
  handleEnvSensorStatus(envSensorStatus, true);

  Serial.println();
  Serial.print(F("BSEC version: "));
  Serial.println(envSensor.getVersion());
  Serial.println();

  /**
   * Clear the LEDs and the display
   */
  leds.clear();
  display.clear();

  /**
   * Set the initial state
   */
  nextAppState();
}

/**
 * @brief Loop
 */
void loop()
{
  // Update sensors
  envSensor.update();
  qtouch.update();

#if BSEC_SAVE_STATE
  // Save BSEC state
  if (millis() - lastBsecStateSave > BSEC_STATE_SAVE_INTERVAL)
  {
    saveBsecState();
    lastBsecStateSave = millis();
  }
#endif

  // Handle touch events
  if (qtouch.onTouchDown(TOUCH0))
  {
    setAppState(APP_STATE_TEMPERATURE, false);
    flashLed(0);
  }

  if (qtouch.onTouchDown(TOUCH1))
  {
    setAppState(APP_STATE_HUMIDITY, false);
    flashLed(1);
  }

  if (qtouch.onTouchDown(TOUCH2))
  {
    nextAppState();
    flashLed(2);
  }

  if (qtouch.onTouchDown(TOUCH3))
  {
    setAppState(APP_STATE_IAQ, false);
    flashLed(3);
  }

  // Handle application state
  uint8_t data;
  switch (appState)
  {
  case APP_STATE_IDLE:
    leds.clear();
    display.clear();
    break;

  case APP_STATE_TEMPERATURE:
    data = env_sensor::temperature;
    displayDataScreen(display.color565(64, 204, 177), bitmap_temperature, String(data) + "'");
    break;

  case APP_STATE_HUMIDITY:
    data = env_sensor::humidity;
    displayDataScreen(display.color565(51, 123, 204), bitmap_droplet, String(data) + "%");
    break;

  case APP_STATE_IAQ:
    data = 100 - env_sensor::iaq * 100 / 500;
    displayDataScreen(display.color565(141, 204, 51), bitmap_leaf, String(data) + "%", "- " + String(env_sensor::iaqAccuracy) + " -");
    break;
  }

  // Auto switch state
  if (appStateAuto && millis() - lastAppStateChange > STATE_TIMEOUT)
    nextAppState();

  delay(10);
}

void halt()
{
  Serial.println(F("Halting..."));
  while (true)
    delay(10);
}

void setAppState(AppState state, bool stateAuto)
{
  appState = state;
  appStateAuto = stateAuto;
  lastAppStateChange = millis();
}

void nextAppState()
{
  switch (appState)
  {
  case APP_STATE_IDLE:
    setAppState(APP_STATE_IAQ);
    break;

  case APP_STATE_IAQ:
    setAppState(APP_STATE_TEMPERATURE);
    break;

  case APP_STATE_TEMPERATURE:
    setAppState(APP_STATE_HUMIDITY);
    break;

  case APP_STATE_HUMIDITY:
    setAppState(APP_STATE_IAQ);
    break;
  }
}

void displayDataScreen(uint16_t background, const byte *icon, String data, String accuracy)
{
  if (lastAppState != appState)
  {
    lastAppState = appState;
    lastData = "";
    lastAccuracy = "";
    display.drawBackground(background);
    display.drawIcon(30, icon, 64, 64, Display::WHITE);
  }

  if (lastData != data)
  {
    display.drawText(160, lastData, 10, background); // Hide previous text
    display.drawText(160, data, 10, Display::WHITE);
    lastData = data;
  }

  if (lastAccuracy != accuracy)
  {
    display.drawText(220, lastAccuracy, 3, background); // Hide previous text
    display.drawText(220, accuracy, 3, Display::WHITE);
    lastAccuracy = accuracy;
  }
}

void flashLed(uint8_t index)
{
  leds.set(index, leds.white);
  delay(50);
  leds.set(index, leds.black);
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

void handleEnvSensorStatus(EnvSensorStatus status, bool showWarning)
{
  if (status == ENV_SENSOR_OK)
    return;

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
  else if (showWarning && bme68xStatus > BME68X_OK)
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
  else if (showWarning && bsecStatus > BSEC_OK)
  {
    leds.warning();
    display.warning("BSEC " + String(bsecStatus));
    delay(3000);
  }
}
