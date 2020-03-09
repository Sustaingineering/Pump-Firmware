#pragma once
#include <Arduino.h>

enum sensorType { digital, analog, soft };

//template <class T>
class sensor 
{
private:
    int m_pin;
    String m_name;
    String m_unit;
    sensorType m_type;
    int m_data;
public:
    sensor(int pin, sensorType type, String name, String unit);
    void initialize();
    int readRaw();
    String read();
};

//template <class T>
sensor::sensor(int pin, sensorType type, String name, String unit)
{
    m_pin = pin;
    m_type = type;
    m_name = name;
    m_unit = unit;
    m_data = -1;
}


//template <class T>
void sensor::initialize()
{
    pinMode(m_pin, INPUT);
}

//template <class T>
int sensor::readRaw()
{   
    switch (m_type)
    {
    case digital:
        return digitalRead(m_pin);
        break;
    case analog:
        return analogRead(m_pin);
        break;
    case soft:
        return m_data++; 
        break;
    default:
        return -1;
        break;
    }
}

String sensor::read()
{
    int raw = readRaw();
    return m_name + String(": ") + String(raw) + String(" (") + m_unit + String(")");
}

// Sensors to be used:
//  Voltage (V)
//  Current (A)
//  Water Flow ()
//  Temperature (C)