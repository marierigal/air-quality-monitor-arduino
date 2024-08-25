#include "EnvSensor.h"

// Initialize the env_sensor instance
env_sensor env_sensor_state;
float env_sensor::temperature;
float env_sensor::rawTemperature;
float env_sensor::humidity;
float env_sensor::rawHumidity;
float env_sensor::rawPressure;
float env_sensor::iaq;
uint8_t env_sensor::iaqAccuracy;
float env_sensor::co2Estimate;
uint8_t env_sensor::co2EstimateAccuracy;
float env_sensor::vocEstimate;
uint8_t env_sensor::vocEstimateAccuracy;
float env_sensor::gasResistance;
uint8_t env_sensor::gasResistanceAccuracy;
uint8_t env_sensor::stabilizationStatus;
uint8_t env_sensor::runInStatus;

EnvSensor::EnvSensor()
{
}

EnvSensor::~EnvSensor()
{
    end();
}

EnvSensorStatus EnvSensor::begin(bool (*userLoadState)(uint8_t *state) /* nullptr */)
{
    if (bsec == nullptr)
    {
        bsec = new Bsec2();
        Wire.begin();

        if (!bsec->begin(BME68X_I2C_ADDR_LOW, Wire))
            return getStatus();

        bsec_virtual_sensor_t sensorList[] = {
            BSEC_OUTPUT_RAW_TEMPERATURE,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
            BSEC_OUTPUT_RAW_HUMIDITY,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_IAQ,
            BSEC_OUTPUT_CO2_EQUIVALENT,
            BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
            BSEC_OUTPUT_RAW_GAS,
            BSEC_OUTPUT_STABILIZATION_STATUS,
            BSEC_OUTPUT_RUN_IN_STATUS,
        };

        // Set config
        if (!bsec->setConfig(bsec_config))
            return getStatus();

        // Set state
        if (userLoadState)
        {
            uint8_t state[EnvSensor::STATE_MAX_SIZE];
            if (userLoadState(state))
                if (!bsec->setState(state))
                    return getStatus();
        }

        // Subscribe to data updates
        if (!bsec->updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_LP))
            return getStatus();

        bsec->attachCallback(newDataAvailable);
    }

    return getStatus();
}

void EnvSensor::end()
{
    delete bsec;
    bsec = nullptr;
}

EnvSensorStatus EnvSensor::update()
{
    if (bsec == nullptr)
        return getStatus();

    bsec->run();

    return getStatus();
}

EnvSensorStatus EnvSensor::getStatus()
{
    int8_t bsecStatus = getBsecStatus();
    int8_t bme68xStatus = getBme68xStatus();

    if (bsecStatus < BSEC_OK || bme68xStatus < BME68X_OK)
        return ENV_SENSOR_ERROR;

    if (bsecStatus > BSEC_OK || bme68xStatus > BME68X_OK)
        return ENV_SENSOR_WARNING;

    return ENV_SENSOR_OK;
}

int8_t EnvSensor::getBsecStatus()
{
    return bsec->status;
}

int8_t EnvSensor::getBme68xStatus()
{
    return bsec->sensor.status;
}

bool EnvSensor::getState(uint8_t *state)
{
    return bsec->getState(state);
}

String EnvSensor::getVersion()
{
    char version[16];
    sprintf(version, "%d.%d.%d.%d", bsec->version.major, bsec->version.minor, bsec->version.major_bugfix, bsec->version.minor_bugfix);
    return version;
}

bsec_version_t EnvSensor::getBsecVersion()
{
    return bsec->version;
}

void EnvSensor::newDataAvailable(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec)
{
    if (!outputs.nOutputs)
        return;

    // int timestamp = (int)(outputs.output[0].time_stamp / INT64_C(1000000));

    for (uint8_t i = 0; i < outputs.nOutputs; i++)
    {
        const bsecData output = outputs.output[i];

        switch (output.sensor_id)
        {
        case BSEC_OUTPUT_RAW_TEMPERATURE:
            env_sensor::rawTemperature = output.signal;
            break;

        case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
            env_sensor::temperature = output.signal;
            break;

        case BSEC_OUTPUT_RAW_HUMIDITY:
            env_sensor::rawHumidity = output.signal;
            break;

        case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
            env_sensor::humidity = output.signal;
            break;

        case BSEC_OUTPUT_RAW_PRESSURE:
            env_sensor::rawPressure = output.signal;
            break;

        case BSEC_OUTPUT_IAQ:
            env_sensor::iaq = output.signal;
            env_sensor::iaqAccuracy = output.accuracy;
            break;

        case BSEC_OUTPUT_CO2_EQUIVALENT:
            env_sensor::co2Estimate = output.signal;
            env_sensor::co2EstimateAccuracy = output.accuracy;
            break;

        case BSEC_OUTPUT_BREATH_VOC_EQUIVALENT:
            env_sensor::vocEstimate = output.signal;
            env_sensor::vocEstimateAccuracy = output.accuracy;
            break;

        case BSEC_OUTPUT_RAW_GAS:
            env_sensor::gasResistance = output.signal;
            env_sensor::gasResistanceAccuracy = output.accuracy;
            break;

        case BSEC_OUTPUT_STABILIZATION_STATUS:
            env_sensor::stabilizationStatus = output.signal;
            break;

        case BSEC_OUTPUT_RUN_IN_STATUS:
            env_sensor::runInStatus = output.signal;
            break;

        default:
            break;
        }
    }
}
