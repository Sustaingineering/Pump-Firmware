/*
 * Content: class that controls Temperature sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include "FarmSensor.h"
#include "Temperature.h"
#include "OneWire.h"
#include "DS18.h"

class Temperature : public FarmSensor
{
private:
    DS18 m_sensor;
    bool checkConnection();
protected:
    float readRaw() override;
public:
    Temperature(int pin);
    bool initialize() override;
};
