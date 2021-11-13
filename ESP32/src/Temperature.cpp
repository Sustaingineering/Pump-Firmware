#include "Temperature.h"

Temperature::Temperature(int pin, sensorType type, String name, String unit, char shortcut): 
                FarmSensor(pin, type, name, unit, shortcut), m_oneWire(pin), m_sensors(&m_oneWire) {}

bool Temperature::initialize()
{
    Serial.println("Initializing Temperature Sensor...");

    m_sensors.begin();

    m_sensors.requestTemperatures();

    float temp = m_sensors.getTempCByIndex(0);

    if ((int) temp == DEVICE_DISCONNECTED_C){
        isWorking = false;
        Serial.println("Temperature Sensor Initialization Failed.");
    }
    else{
        isWorking = true;
        Serial.println("Temperature Sensor Initialized.");
    }

    return isWorking;
}

float Temperature::readRaw()
{
    if (isWorking == false)
        return nanf("0");

    m_sensors.requestTemperatures();

    float temp = m_sensors.getTempCByIndex(0);

    if ((int) temp == DEVICE_DISCONNECTED_C)
    {    
        Serial.print("TEMPERATURE SENSOR IS DISCONNECTED\n");
        isWorking = false;
        return nanf("0");
    }
    else
        return temp;
}
