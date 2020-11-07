#include "Counter.h"

Counter::Counter(int pin,  String name, String unit, char shortcut):farmSensor(pin, name, unit, shortcut)
{
    m_data = 1.0 * pin;
}

void Counter::initialize()
{
    /*Nothing to initialize*/
}

float Counter::readRaw()
{
    return m_data + 0.1;
}