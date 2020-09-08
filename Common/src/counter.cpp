#include "counter.h"

counter::counter(int pin,  String name, String unit, char shortcut):farmSensor(pin, name, unit, shortcut)
{
    m_data = 1.0 * pin;
}

void counter::initialize()
{
    /*Nothing to initialize*/
}

float counter::readRaw()
{
    return m_data + 0.1;
}