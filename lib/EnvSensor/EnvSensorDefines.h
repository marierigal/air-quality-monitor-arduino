#ifndef _ENVSENSOR_DEFINES_
#define _ENVSENSOR_DEFINES_

// Sensor library
#include "bsec2.h"

#include <Leds.h>
#include "Display.h"

/**
 * @brief BSEC configuration
 */
const uint8_t bsec_config[] = {
#include "config/bme688_sel_33v_3s_4d.txt"
};

class env_sensor
{
public:
    static float temperature;
    static float rawTemperature;
    static float humidity;
    static float rawHumidity;
    static float rawPressure;
    static float iaq;
    static uint8_t iaqAccuracy;
    static float co2Estimate;
    static uint8_t co2EstimateAccuracy;
    static float vocEstimate;
    static uint8_t vocEstimateAccuracy;
    static float gasResistance;
    static uint8_t gasResistanceAccuracy;
    static uint8_t stabilizationStatus;
    static uint8_t runInStatus;
};

/**
 * @brief EnvSensor instance
 */
extern env_sensor env_sensor_instance;

#endif
