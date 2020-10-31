
 
/*
 * Content: class that measures current using the ACS711EX Hall Effect Sensor
 * Author : Forbes Choy
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>
#include <farmSensor.h>
 
 
class current: public farmSensor
{
private:
     //constants declarations (cannot declare static const floats in .h files, need .cpp --> 3.9 defined in readRaw())
    static const int adc_res = 4095; //ESP32 ADC resolution is 12 bits
    float m_MaxV; //Maximum Analog Voltage (specific to each board)
protected:
    float readRaw() override; //returns currentage of source (NOT THE DIVIDER) in decimal value
public: 
    //functions
    current(int pin, String name, String unit, char shortcut, float maxV);
    void initialize() override;
};
