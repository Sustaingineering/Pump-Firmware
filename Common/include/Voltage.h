
/*
 * Content: class that measures voltage using voltage dividers
 * Author : Forbes Choy
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include <farmSensor.h>

class Voltage : public farmSensor 
{
private:
    int m_higherResistor; // voltage divider, higher resistor value [ohms]
    int m_lowerResistor; // voltage divider, lower resistor value [ohms]
    float m_maximumVoltage; // Maximum Voltage (specific to board)
protected:
    float readRaw() override; 
public:
    static const int analogDigitalConversionResolution = 4095; // ESP32 ADC resolution is 12 bits
    Voltage(int pin, sensorType type, String name, String unit, char shortcut, int lowerResistor, int higherResistor, float maximumVoltage);
    void initialize() override;
};

