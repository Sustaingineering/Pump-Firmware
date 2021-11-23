#include "Temperature.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_TEMPERATURE_SWITCH

Temperature::Temperature(bool isConnected, int pin): 
                FarmSensor(isConnected, pin), m_oneWire(pin), m_sensors(&m_oneWire) {}

bool Temperature::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }

    m_sensors.begin();

    m_sensors.requestTemperatures();

    float temp = m_sensors.getTempCByIndex(0);

    if ((int) temp == DEVICE_DISCONNECTED_C){
        isWorking = false;
        LOGGER("Temperature Sensor Initialization Failed.");
    }
    else{
        isWorking = true;
        LOGGER("Temperature Sensor Initialized.");
    }

    return isWorking;
}

float Temperature::readRaw()
{
    if (!m_isConnected)
    {
        return count();
    }

    if (isWorking == false)
        return nanf("0");

    m_sensors.requestTemperatures();

    float temp = m_sensors.getTempCByIndex(0);

    if ((int) temp == DEVICE_DISCONNECTED_C)
    {    
        LOGGER("TEMPERATURE SENSOR IS DISCONNECTED");
        isWorking = false;
        return nanf("0");
    }
    else
        return temp;
}
