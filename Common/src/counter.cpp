#include "counter.h"

counter::counter(int pin, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
    m_data = 0.0;
}

void counter::initialize()
{
    /*Nothing to initialize*/
}

float counter::readRaw()
{
    return m_data + 0.1;
}