#include "Counter.h"

static int pin = 0;
static char shortcut = 'a';

Counter::Counter() {
    m_name = "Counter";
    m_pin = pin++;
    m_unit = 'T';
    m_shortcut = shortcut++;
    m_data = 1.0 * m_pin;
}

Counter::Counter(int pin,  String name, String unit, char shortcut):
    FarmSensor(pin, name, unit, shortcut)
{
    m_data = 1.0 * m_pin;
}

bool Counter::initialize()
{
    /*Nothing to initialize*/
    isWorking = true;
    Serial.printf("Counter %d is initialized\n", m_pin);
    return isWorking;
}

float Counter::readRaw()
{
    return m_data + 0.1;
}