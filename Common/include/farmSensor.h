/*
 * Content: class that controls generic sensors, analog, digital and simulated sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include "IFarmSensor.h"

class FarmSensor : public IFarmSensor
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
    FarmSensor(int pin, sensorType type, String name, String unit, char shortcut);
    FarmSensor(int pin, String name, String unit, char shortcut);
    void initialize() override;
    String read() override;
    packet pack() override;
};

// Sensors to be used:
//  Voltage (V)
//  Current (A)
//  Water Flow ()
//  Temperature (C)