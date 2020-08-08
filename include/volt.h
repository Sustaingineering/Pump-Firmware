
/*
 * Content: class that measures voltage using voltage dividers
 * Author : Forbes Choy
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include <farmSensor.h>

class volt : public farmSensor {
private:
    int m_RH; // voltage divider, higher resistor value [ohms]
    int m_RL; // voltage divider, lower resistor value [ohms]
protected:
    float readRaw() override; 
public:
    static const int adc_res = 4095; // ESP32 ADC resolution is 12 bits
    volt(int pin, sensorType type, String name, String unit, char shortcut, int RL, int RH);
    void initialize() override;
};

/* constructor */
volt::volt(int pin, sensorType type, String name, String unit, char shortcut, int RL, int RH): 
            farmSensor(pin, type, name, unit, shortcut), m_RH(RH), m_RL(RL) {}

/* overriding */
void volt::initialize()
{
    pinMode(m_pin, INPUT);
}


/* overriding */
float volt::readRaw(){
    return analogRead(m_pin) * (3.9 / adc_res) * ((m_RH + m_RL) / m_RL);
}


