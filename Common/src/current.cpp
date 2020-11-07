#include "Current.h"

Current::Current(int pin, String name, String unit, char shortcut, float maximumVoltage):
    farmSensor(pin,name,unit,shortcut),m_maximumVoltage(maximumVoltage)
{
}
 
void Current::initialize()
{
    pinMode(m_pin, INPUT);
}
 
float Current::readRaw()
{
    int analogData = analogRead(m_pin); // Take reading
    // Convert to decimal
    float HallVoltage = analogData * (m_maximumVoltage / analogDigitalConversionResolution);
    // Compute the current from the voltage reading equation
    float HallAmps = (HallVoltage * 22.0) / 3.0 - (55.0 / 3.0);
 
 
    //cannot define static const float values in header files --> needs to be in the .cpp file
    //return analogRead(pin) * (3.9 / adc_res) * ((RH + RL) / RL); 
    return HallAmps;
}
 