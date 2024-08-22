#ifndef _ENVSENSOR_H_INCLUDED
#define _ENVSENSOR_H_INCLUDED

// Sensor library
#include <bsec2.h>

// Dependencies
#include <Leds.h>
#include <Display.h>

/**
 * @brief BSEC configuration
 */
const uint8_t bsec_config[] = {
#include "config/bme688_sel_33v_3s_4d.txt"
};

/**
 * @brief Sensor state
 */
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
extern env_sensor env_sensor_state;

class EnvSensor
{
public:
    EnvSensor();
    ~EnvSensor();

    /**
     * @brief Initialize the instance
     */
    bool begin();

    /**
     * @brief Destroy the instance
     */
    void end();

    /**
     * @brief Update sensor data
     */
    void update();

    /**
     * @brief : This function is called by the BSEC library when a new output is available
     * @param[in] data      : BME68X sensor data before processing
     * @param[in] outputs   : Processed BSEC BSEC output data
     * @param[in] bsec      : Instance of BSEC2 calling the callback
     */
    static void newDataAvailable(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

protected:
    /**
     * @brief BSEC library instance
     */
    Bsec2 *bsec = nullptr;

    /**
     * @brief Error function
     * @param message The message to display
     * @param code The error code
     */
    void error(const char *message, uint8_t code);

    /**
     * @brief Warning function
     * @param message The message to display
     * @param code The warning code
     */
    void warning(const char *message, uint8_t code);

    /**
     * @brief Get BSEC version
     */
    String getBsecVersion();

    /**
     * @brief Check sensor status
     */
    void checkSensorStatus();
};

#endif //_ENVSENSOR_H_INCLUDED
