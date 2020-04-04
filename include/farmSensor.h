/*
 * Content: class that controls generic sensors, analog, digital and simulated sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include "LoRaTransceiver.h"

enum sensorType { digital, analog, counter };

//template <class T>
class farmSensor 
{
private:
    int m_pin;
    String m_name;
    String m_unit;
    sensorType m_type;
    char m_shortcut;
    int m_data;
    int readRaw();
public:
    farmSensor(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize();
    String read();
    packet pack();
};

//template <class T>
farmSensor::farmSensor(int pin, sensorType type, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_type = type;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
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
    case counter:
        return m_data++; 
        break;
    default:
        return -1;
        break;
    }
}

String farmSensor::read()
{
    m_data = readRaw();
    return m_name + String(": ") + String(m_data) + String(" (") + m_unit + String(")") + String(" | ");
}

packet farmSensor::pack()
{
    packet ret;
    ret.type = m_shortcut;
    ret.data = m_data;
    return ret;
}

// Sensors to be used:
//  Voltage (V)
//  Current (A)
//  Water Flow ()
//  Temperature (C)