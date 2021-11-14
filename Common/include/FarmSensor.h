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
    int m_pin;
    float m_data;
    bool isWorking;
    virtual float readRaw() = 0;
public:
    FarmSensor();
    FarmSensor(int pin);
    virtual bool initialize() = 0;
    String read();
};
