#ifndef _ENVSENSOR_H_INCLUDED
#define _ENVSENSOR_H_INCLUDED

// Sensor library
#include <bsec2.h>

/**
 * @brief BSEC configuration
 */
const uint8_t bsec_config[] = {
#include "config/bme688_sel_33v_3s_4d.txt"
};

typedef enum
{
    ENV_SENSOR_ERROR = -1,
    ENV_SENSOR_OK = 0,
    ENV_SENSOR_WARNING = 1,
} EnvSensorStatus;

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
    EnvSensorStatus begin();

    /**
     * @brief Destroy the instance
     */
    void end();

    /**
     * @brief Update sensor data
     * @return EnvSensorStatus The status
     */
    EnvSensorStatus update();

    /**
     * @brief Get BSEC and sensor status
     * @return EnvSensorStatus The status
     */
    EnvSensorStatus getStatus();

    /**
     * @brief Get BSEC status
     * @return BSEC status
     *  if neggative there is an error,
     *  if positive there is a warning,
     *  if 0 everything is ok
     */
    int8_t getBsecStatus();

    /**
     * @brief Get sensor status
     * @return Sensor status
     *  if neggative there is an error,
     *  if positive there is a warning,
     *  if 0 everything is ok
     */
    int8_t getBme68xStatus();

    /**
     * @brief Get BSEC version
     * @return The version
     */
    String getVersion();

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
     * @brief Get BSEC version
     * @return The version
     */
    bsec_version_t getBsecVersion();
};

#endif //_ENVSENSOR_H_INCLUDED
