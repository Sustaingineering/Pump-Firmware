/*
 * Content: class that controls generic sensors, analog, digital and simulated sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include "LoRaTransceiver.h"

enum sensorType { digital, analog };

//template <class T>
class farmSensor 
{
private:
    sensorType m_type;
protected:
    int m_pin;
    String m_name;
    String m_unit;
    char m_shortcut;
    float m_data;
    virtual float readRaw();
public:
    farmSensor(){}
    farmSensor(int pin, sensorType type, String name, String unit, char shortcut);
    virtual void initialize();
    virtual String read(); // unsure if virtual is needed
    virtual packet pack();
};

//template <class T>
farmSensor::farmSensor(int pin, sensorType type, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_type = type;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
    m_data = 0.0;
}


//template <class T>
void farmSensor::initialize()
{
    pinMode(m_pin, INPUT);
}

//template <class T>
float farmSensor::readRaw()
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