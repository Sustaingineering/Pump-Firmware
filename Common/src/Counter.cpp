#include "Counter.h"

#include "Counter.h"

Counter ** Counter::createCounters(int counters)
{
    char letter = 'a';
    Counter **counterArray = (Counter**) malloc(sizeof(Counter*) * counters);
    for (int j = 0; j < counters; j++)
    {
        String name = "Counter";
        name += j;
        counterArray[j] = new Counter(j, name, "T", letter); 
        letter += 1;
    }

    return counterArray;
}

Counter::Counter(int pin,  String name, String unit, char shortcut):
    FarmSensor(pin, name, unit, shortcut)
{
    m_data = 1.0 * pin;
}

bool Counter::initialize()
{
    /*Nothing to initialize*/
    isWorking = true;
    return isWorking;
}

float Counter::readRaw()
{
    return m_data + 0.1;
}