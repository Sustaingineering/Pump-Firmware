#include "counter.h"

counter ** counter::createCounters(int counters)
{
    char letter = 'a';
    counter **counterArray = (counter **) malloc(sizeof(counter *) * counters);
    for (int j = 0; j < counters; j++)
    {
        String name = "Counter";
        name += j;
        counterArray[j] = new counter(j, name, "T", letter); 
        letter += 1;
    }

    return counterArray;
}

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