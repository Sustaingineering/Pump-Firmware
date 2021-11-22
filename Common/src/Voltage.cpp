#include "Voltage.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_VOLTAGE_SWITCH

/* constructor */
Voltage::Voltage(bool isConnected, int pin, int lowerResistor, int higherResistor, float maximumVoltage): 
    FarmSensor(isConnected, pin), m_higherResistor(higherResistor), m_lowerResistor(lowerResistor), m_maximumVoltage(maximumVoltage)
{    
}

/* overriding */
bool Voltage::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }
    pinMode(m_pin, INPUT);
    isWorking = true;
    LOGGER("Initialized voltage sensor");
    return isWorking;
}


/* overriding */
float Voltage::readRaw()
{
    if (!m_isConnected)
    {
        return count();
    }
    return analogRead(m_pin) * (m_maximumVoltage / analogDigitalConversionResolution) * ((m_higherResistor + m_lowerResistor) / m_lowerResistor);
}