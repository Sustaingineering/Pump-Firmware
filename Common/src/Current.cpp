#include "Current.h"
#define I_PN 10 // can be ±10, ±20, ±30, ±50, ±100, ±150, or ±200

Current::Current(bool isConnected, int pin, float maximumVoltage):
    FarmSensor(isConnected, pin),m_maximumVoltage(maximumVoltage)
{
}
 
bool Current::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }
    Serial.println("Initializing Current Sensor...");
    pinMode(m_pin, INPUT);
    isWorking = true;
    Serial.println("Initialized Current Sensor");
    return isWorking;
}
 
float Current::readRaw()
{
    if (!m_isConnected)
    {
        return count();
    }
    int analogData = analogRead(m_pin); // Take reading
    // Convert to decimal
    float hallVoltage = analogData * (m_maximumVoltage / analogDigitalConversionResolution);
    // Compute the current from the voltage reading equation
    float hallAmps = 0;
    if (hallVoltage > 2.5)
        hallAmps = (hallVoltage - 2.5) * (I_PN / 0.625);
    else if (hallVoltage < 2.5)
        hallAmps = (2.5 - hallVoltage) * (I_PN / 0.625);
    return hallAmps;
}
 