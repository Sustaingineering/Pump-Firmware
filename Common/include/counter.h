/*
 * Content: a virtual objects that generate psuedo data for debugging purposes.
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>
#include "farmSensor.h"

class counter : public farmSensor
{
protected:
    float readRaw() override;
public:
    counter(int pin, String name, String unit, char shortcut);
    void initialize() override;
};

counter::counter(int pin, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
    m_data = 0.0;
}

void counter::initialize()
{
    /*Nothing to initialize*/
}

float counter::readRaw()
{
    return m_data + 0.1;
}