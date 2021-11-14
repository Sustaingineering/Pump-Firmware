#include "FarmSensor.h"

FarmSensor::FarmSensor() {
    
}

FarmSensor::FarmSensor(int pin)
{
    m_pin = pin;
    m_data = 0.0;
}

String FarmSensor::read()
{
    m_data = readRaw();
    return String(m_data, 2) + String(",");
}
