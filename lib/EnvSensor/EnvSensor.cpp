#include <Leds.h>

#include "EnvSensor.h"

env_sensor env_sensor_instance;
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

bool EnvSensor::begin()
{
    if (bsec == nullptr)
    {
        bsec = new Bsec2();
        Wire.begin();

        if (!bsec->begin(BME68X_I2C_ADDR_LOW, Wire))
            checkSensorStatus();

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

        if (!bsec->updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_LP))
            checkSensorStatus();

        bsec->attachCallback(newDataAvailable);

        Serial.println();
        Serial.print(F("BSEC version: "));
        Serial.println(getBsecVersion());
        Serial.println();
    }

    return true;
}

void EnvSensor::end()
{
    delete bsec;
    bsec = nullptr;
}

void EnvSensor::update()
{
    if (bsec == nullptr)
        return;

    if (!bsec->run())
        checkSensorStatus();
}

void EnvSensor::error()
{
    while (true)
    {
        leds.error();
        delay(500);
        leds.clear();
        delay(500);
    }
}

void EnvSensor::warning()
{
    leds.warning();
}

void EnvSensor::checkSensorStatus()
{
    uint8_t status = bsec->status;
    if (status < BSEC_OK)
    {
        Serial.print(F("[BSEC] Error: "));
        Serial.print(status);
        Serial.println();
        error();
    }
    else if (status > BSEC_OK)
    {
        Serial.print(F("[BSEC] Warning: "));
        Serial.print(status);
        Serial.println();
        warning();
    }

    uint8_t sensor_status = bsec->sensor.status;
    if (sensor_status < BME68X_OK)
    {
        Serial.print(F("[BME68X] Error: "));
        Serial.print(sensor_status);
        Serial.println();
        error();
    }
    else if (sensor_status > BME68X_OK)
    {
        Serial.print(F("[BME68X] Warning: "));
        Serial.print(sensor_status);
        Serial.println();
        warning();
    }
}

String EnvSensor::getBsecVersion()
{
    String version;
    version.concat(bsec->version.major);
    version.concat(F("."));
    version.concat(bsec->version.minor);
    version.concat(F("."));
    version.concat(bsec->version.major_bugfix);
    version.concat(F("."));
    version.concat(bsec->version.minor_bugfix);
    return version;
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
