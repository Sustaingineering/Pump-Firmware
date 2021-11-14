#include "Flow.h"

namespace Water
{
    unsigned int numberOfFanTops;
    void countFlowInterrupts()
    {
        numberOfFanTops++;
    }
}

Flow::Flow(int pin):
        FarmSensor(pin) {}
 
bool Flow::initialize()
{
    Serial.println("Initializing Water Flow Sensor");
    pinMode(m_pin, INPUT); //initializes digital pin as an input
    attachInterrupt(digitalPinToInterrupt(m_pin), Water::countFlowInterrupts, RISING); //and the interrupt is attached
    isWorking = true;
    Serial.println("Initialized Water Flow Sensor.");
    return isWorking;
}

float Flow::readRaw()
{
    Water::numberOfFanTops = 0;   

    sei(); //Enables interrupts
    delay (1000); //Wait 1 second
    cli(); //Disable interrupts

    //calculations subtract number of times called outside of interupt
    int flowRate = (Water::numberOfFanTops * 60 / FLOW_RATE_FACTOR); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/min
    Serial.printf("Flow Rate: %d L/min\n",flowRate); //Prints flow rate calculated above
    Serial.printf("numberOfFanTops: %d \n", Water::numberOfFanTops);
    Serial.print("\n");
    return flowRate;
}
