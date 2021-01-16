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
    static counter ** createCounters(int counters);
};
