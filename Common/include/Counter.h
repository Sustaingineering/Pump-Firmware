/*
 * Content: a virtual objects that generate psuedo data for debugging purposes.
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>
#include "FarmSensor.h"

class Counter : public FarmSensor
{
protected:
    float readRaw() override;
public:
    Counter();
    Counter(int pin);
    bool initialize() override;
};
