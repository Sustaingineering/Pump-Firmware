#include "FarmSensor.h"

FarmSensor::FarmSensor() {
    
}

//template <class T>
FarmSensor::FarmSensor(int pin, sensorType type, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_type = type;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
    m_data = 0.0;
}

FarmSensor::FarmSensor(int pin, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
    m_data = 0.0;
}


//template <class T>
bool FarmSensor::initialize()
{
    pinMode(m_pin, INPUT);
    isWorking = true;
    return isWorking;
}

//template <class T>
float FarmSensor::readRaw()
{   
    switch (m_type)
    {
    case digital:
        return (float)digitalRead(m_pin);
    case analog:
        return (float)analogRead(m_pin);
    default:
        return nanf("Unexpected sensor type");
    }
}

String FarmSensor::read()
{
    m_data = readRaw();
    // return m_name + String(": ") + String(m_data) + String(" (") + m_unit + String(")") + String(" | ");
    return String(m_data, 2) + String(",");
}

packet FarmSensor::pack()
{
    packet ret;
    ret.type = m_shortcut;
    ret.data = m_data;
    return ret;
}