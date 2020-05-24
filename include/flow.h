/*
 * Content: reads water flow sensor and creates packets, returns string and package
 * Author : Ryan Leung
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include "LoRaTransceiver.h"
#include "farmSensor.h"

enum sensorType { digital, analog, counter };

class flow : public farmSensor{

private:
    int m_pin;
    String m_name;
    String m_unit;
    sensorType m_type;
    char m_shortcut;
    int m_data;
    int readRaw();
public:
    flow(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize();
    String read();
    packet pack();
};

flow::flow(int pin, sensorType type, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_type = type;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
    m_data = -1;
}

void flow::initialize()
{
    pinMode(m_pin, INPUT); //initializes digital pin as an input
    attachInterrupt(digitalPinToInterrupt(m_pin),NbTopsCounterCaller, RISING); //and the interrupt is attached
}

//Calls counter function
void NbTopsCounterCaller(){
    NbTopsCounter();
}

//returns number of times that function is called
int NbTopsCounter(){
    static int NbTopsFan=0;
    return ++NbTopsFan;
}

int readRaw()
{   
  sei(); //Enables interrupts
  delay (1000); //Wait 1 second
  cli(); //Disable interrupts
//calculations subtract number of times called outside of interupt
  int Calc = ((NbTopsCounter()-1)* 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/min
  Serial.print (Calc, DEC); //Prints the number calculated above
  Serial.print (" L/min\r\t"); //Prints "L/hour" and returns a new line
  Serial.print ("NbTopsFan:");
  Serial.print ((NbTopsCounter()-2),DEC);
  Serial.print ("\n");
  return Calc;
}

String flow::read(){
    m_data = readRaw();
    return m_name + String(": ") + String(m_data) + String(" (") + m_unit + String(")") + String(" | ");
}

packet flow::pack()
{
    packet ret;
    ret.type = m_shortcut;
    ret.data = m_data;
    return ret;
}




