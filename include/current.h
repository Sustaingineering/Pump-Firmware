
 
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
protected:
    float readRaw() override; //returns currentage of source (NOT THE DIVIDER) in decimal value
public: 
    //functions
    current(int pin, String name, String unit, char shortcut);
    void initialize() override;
};
 
current::current(int pin, String name, String unit, char shortcut)
{
  m_pin = pin;
  m_name = name;
  m_unit = unit;
  m_shortcut = shortcut;
  m_data = 0.0;
}
 
 
void current::initialize()
{
    pinMode(m_pin, INPUT);
}
 
float current::readRaw()
{
  int x = analogRead(m_pin); // Take reading
  // Convert to decimal
  float HallVoltage = x * (3.9 / adc_res);
  // Compute the current from the voltage reading equation
  float HallAmps = (HallVoltage * 22.0) / 3.0 - (55.0 / 3.0);
 
 
  //cannot define static const float values in header files --> needs to be in the .cpp file
//   return analogRead(pin) * (3.9 / adc_res) * ((RH + RL) / RL); 
  return HallAmps;
}
 

