#include "Current.h"

Current::Current(int pin, String name, String unit, char shortcut, float maximumVoltage):
    FarmSensor(pin,name,unit,shortcut),m_maximumVoltage(maximumVoltage)
{
}
 
bool Current::initialize()
{
    Serial.println("Initializing Current Sensor...");
    pinMode(m_pin, INPUT);
    isWorking = true;
    Serial.println("Initialized Current Sensor");
    return isWorking;
}
 
float Current::readRaw()
{
    int analogData = analogRead(m_pin); // Take reading
    // Convert to decimal
    float hallVoltage = analogData * (m_maximumVoltage / analogDigitalConversionResolution);
    // Compute the current from the voltage reading equation
    float hallAmps = (hallVoltage * 22.0) / 3.0 - (55.0 / 3.0);
 
 
    //cannot define static const float values in header files --> needs to be in the .cpp file
    //return analogRead(pin) * (3.9 / adc_res) * ((RH + RL) / RL); 
    return hallAmps;
}
 