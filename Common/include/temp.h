/*
 * Content: class that controls Temperature sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include "FarmSensor.h"

class temp : public IFarmSensor
{
private:
    class Impl;
    Impl* m_pImpl;
public: 
    temp(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize() override;
    String read() override;
    packet pack() override;
};
