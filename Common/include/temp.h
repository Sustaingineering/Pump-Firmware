/*
 * Content: class that controls Temperature sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <OneWire.h> 
#include <DallasTemperature.h>
#include "farmSensor.h"

class temp: public farmSensor{
private:
    OneWire m_oneWire;
    DallasTemperature m_sensors; 
protected:
    float readRaw() override;
public: 
    temp(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize() override;
};
