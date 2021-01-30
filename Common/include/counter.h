/*
 * Content: a virtual objects that generate psuedo data for debugging purposes.
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>
#include "farmSensor.h"

class Counter : public farmSensor
{
protected:
    float readRaw() override;
public:
    Counter(int pin, String name, String unit, char shortcut);
    void initialize() override;
    static counter ** createCounters(int counters);
};
