#include "volt.h"

/* constructor */
volt::volt(int pin, sensorType type, String name, String unit, char shortcut, int RL, int RH, float MaxV): 
            farmSensor(pin, type, name, unit, shortcut), m_RH(RH), m_RL(RL), m_MaxV(MaxV){}

/* overriding */
void volt::initialize()
{
    pinMode(m_pin, INPUT);
}


/* overriding */
float volt::readRaw(){
    return analogRead(m_pin) * (m_MaxV / adc_res) * ((m_RH + m_RL) / m_RL);
}