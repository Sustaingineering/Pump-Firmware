#include "FarmSensor.h"

FarmSensor::FarmSensor(bool isConnected, int pin)
{
    m_isConnected = isConnected;
    m_pin = pin;
    m_data = m_pin;
}

String FarmSensor::read()
{
    m_data = readRaw();
    return String(m_data, 2) + String(",");
}

float FarmSensor::count()
{
    m_data = m_data + 0.1;
    return m_data;
}
