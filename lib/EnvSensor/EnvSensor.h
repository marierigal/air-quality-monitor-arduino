#ifndef _ENVSENSOR_H_INCLUDED
#define _ENVSENSOR_H_INCLUDED

#include "EnvSensorDefines.h"

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
