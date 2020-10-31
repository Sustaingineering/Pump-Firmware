
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
    float m_MaxV; // Maximum Voltage (specific to board)
protected:
    float readRaw() override; 
public:
    static const int adc_res = 4095; // ESP32 ADC resolution is 12 bits
    volt(int pin, sensorType type, String name, String unit, char shortcut, int RL, int RH, float MaxV);
    void initialize() override;
};

