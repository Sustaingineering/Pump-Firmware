#include "Flow.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_FLOW_SWITCH

namespace Water
{
    unsigned int numberOfFanTops;
    void countFlowInterrupts()
    {
        numberOfFanTops++;
    }
}

Flow::Flow(bool isConnected, int pin):
        FarmSensor(isConnected, pin) {}
 
bool Flow::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }
    pinMode(m_pin, INPUT); //initializes digital pin as an input
    attachInterrupt(digitalPinToInterrupt(m_pin), Water::countFlowInterrupts, RISING); //and the interrupt is attached
    isWorking = true;
    LOGGER("Initialized Water Flow Sensor");
    return isWorking;
}

float Flow::readRaw()
{
    if (!m_isConnected)
    {
        delay (1000);
        return count();
    }

    Water::numberOfFanTops = 0;   
    interrupts();
    delay (1000); //Wait 1 second
    noInterrupts();
    //calculations subtract number of times called outside of interupt
    //(Pulse frequency x 60) / 5.5Q, = flow rate in L/min
    int flowRate = (Water::numberOfFanTops * 60 / FLOW_RATE_FACTOR);
    return flowRate;
}
