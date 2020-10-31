#include "current.h"

current::current(int pin, String name, String unit, char shortcut, float maxV):
        farmSensor(pin,name,unit,shortcut),m_MaxV(maxV)
{}
 
 
void current::initialize()
{
    pinMode(m_pin, INPUT);
}
 
float current::readRaw()
{
  int x = analogRead(m_pin); // Take reading
  // Convert to decimal
  float HallVoltage = x * (m_MaxV / adc_res);
  // Compute the current from the voltage reading equation
  float HallAmps = (HallVoltage * 22.0) / 3.0 - (55.0 / 3.0);
 
 
  //cannot define static const float values in header files --> needs to be in the .cpp file
//   return analogRead(pin) * (3.9 / adc_res) * ((RH + RL) / RL); 
  return HallAmps;
}
 