#include "Voltage.h"

/* constructor */
Voltage::Voltage(int pin, sensorType type, String name, String unit, char shortcut, int lowerResistor, int higherResistor, float maximumVoltage): 
    FarmSensor(pin, type, name, unit, shortcut), m_higherResistor(higherResistor), m_lowerResistor(lowerResistor), m_maximumVoltage(maximumVoltage)
{    
}

/* overriding */
bool Voltage::initialize()
{
    pinMode(m_pin, INPUT);
    isWorking = true;
    return isWorking;
}


/* overriding */
float Voltage::readRaw()
{
    return analogRead(m_pin) * (m_maximumVoltage / analogDigitalConversionResolution) * ((m_higherResistor + m_lowerResistor) / m_lowerResistor);
}