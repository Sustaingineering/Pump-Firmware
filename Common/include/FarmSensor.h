/*
 * Content: class that controls generic sensors, analog, digital and simulated sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>

class FarmSensor
{
protected:
    bool m_isConnected;
    int m_pin;
    float m_data;
    bool isWorking;
    float count();
    virtual float readRaw() = 0;
public:
    FarmSensor(bool isConnected, int pin);
    virtual bool initialize() = 0;
    String read();
};
