/*
 * Content: reads water flow sensor and creates packets, returns string and package
 * Author : Ryan Leung
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>
#include "farmSensor.h"
 
class flow : public farmSensor{
 
private:
    int m_pin;
    String m_name;
    String m_unit;
    sensorType m_type;
    char m_shortcut;
    int m_data;
    float readRaw() ;
    void NbTopsCounter();
    int NbTopsFan;
public:
    flow(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize() override;
};
 
flow::flow(int pin, sensorType type, String name, String unit, char shortcut)
{
    m_pin = pin;
    m_type = type;
    m_name = name;
    m_unit = unit;
    m_shortcut = shortcut;
    m_data = -1;
    NbTopsFan = 0;
}
 
void flow::initialize()
{
    pinMode(m_pin, INPUT); //initializes digital pin as an input
    attachInterrupt(digitalPinToInterrupt(m_pin),NbTopsCounter, RISING); //and the interrupt is attached
   
}
 
 
//increments NbTopsFans 
void flow::NbTopsCounter(){
   ++NbTopsFan;
}
 
float flow::readRaw()
{   
  sei(); //Enables interrupts
  delay (1000); //Wait 1 second
  cli(); //Disable interrupts
//calculations subtract number of times called outside of interupt
  int Calc = ((NbTopsFan)* 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/min
  Serial.print (Calc,DEC); //Prints the number calculated above
  Serial.print (" L/min\r\t"); //Prints "L/hour" and returns a new line
  Serial.print ("NbTopsFan:");
  Serial.print ((NbTopsFan,DEC);
  Serial.print ("\n");
  return Calc;
}
 
 
 
 
 

