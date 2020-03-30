/*
 * This header file define the class that controls generic sensors, analog, digital and simulated sensor
 * Tarek Al Ayoubi
 * Sustaingeering UBC
 */

#pragma once
#include <Arduino.h>

enum sensorType { digital, analog, soft };

//template <class T>
class farmSensor 
{
private:
    int m_pin;
    String m_name;
    String m_unit;
    sensorType m_type;
    int m_data;
    int readRaw();
public:
    farmSensor(int pin, sensorType type, String name, String unit);
    void initialize();
    String read(int *p_data);
};

//template <class T>
farmSensor::farmSensor(int pin, sensorType type, String name, String unit)
{
    m_pin = pin;
    m_type = type;
    m_name = name;
    m_unit = unit;
    m_data = -1;
}


//template <class T>
void farmSensor::initialize()
{
    pinMode(m_pin, INPUT);
}

//template <class T>
int farmSensor::readRaw()
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

String farmSensor::read(int *p_data)
{
    int raw = readRaw();
    *p_data = raw;
    return m_name + String(": ") + String(raw) + String(" (") + m_unit + String(")") + String(" | ");
}

// Sensors to be used:
//  Voltage (V)
//  Current (A)
//  Water Flow ()
//  Temperature (C)