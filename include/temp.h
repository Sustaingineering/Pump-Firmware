/*
 * Content: class that controls Temperature sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <OneWire.h> 
#include <DallasTemperature.h>

class temp
{
private:
    int m_pin;
    OneWire m_oneWire;
    DallasTemperature m_sensors;
public:
    temp(int pin);
    void initialize();
    float readRaw();
};

temp::temp(int pin): m_oneWire(pin), m_sensors(&m_oneWire)
{
}

void temp::initialize()
{
    m_sensors.begin();
}

float temp::readRaw()
{
    m_sensors.requestTemperatures();
    return m_sensors.getTempCByIndex(0);
}
