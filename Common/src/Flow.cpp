#include "Flow.h"

namespace Water
{
    unsigned int numberOfFanTops;
    void numberOfFanTopsCounter()
    {
        numberOfFanTops++;
    }
}

Flow::Flow(int pin,  String name, String unit, char shortcut):FarmSensor(pin,name,unit,shortcut)
{
}
 
void Flow::initialize()
{
    pinMode(m_pin, INPUT); //initializes digital pin as an input
    attachInterrupt(digitalPinToInterrupt(m_pin), Water::numberOfFanTopsCounter, RISING); //and the interrupt is attached
}

float Flow::readRaw()
{
    Water::numberOfFanTops = 0;   
    sei(); //Enables interrupts
    delay (1000); //Wait 1 second
    cli(); //Disable interrupts
    //calculations subtract number of times called outside of interupt
    int flowRate = (Water::numberOfFanTops * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/min
    Serial.print (flowRate,DEC); //Prints flow rate calculated above
    Serial.print (" L/min\r\t"); //Prints "L/hour" and returns a new line
    Serial.print ("numberOfFanTops:");
    Serial.print (Water::numberOfFanTops, DEC);
    Serial.print ("\n");
    return flowRate;
}
