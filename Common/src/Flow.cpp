#include "Flow.h"

namespace water
{
  unsigned int NbTopsFan;
  void NbTopsCounter()
  {
    NbTopsFan++;
  }
}

flow::flow(int pin, String name, String unit, char shortcut)
{
  m_pin = pin;
  m_name = name;
  m_unit = unit;
  m_shortcut = shortcut;
  m_data = 0.0;
}
 
void flow::initialize()
{
    pinMode(m_pin, INPUT); //initializes digital pin as an input
    attachInterrupt(digitalPinToInterrupt(m_pin), water::NbTopsCounter, RISING); //and the interrupt is attached
}

float flow::readRaw()
{
  water::NbTopsFan = 0;   
  sei(); //Enables interrupts
  delay (1000); //Wait 1 second
  cli(); //Disable interrupts
//calculations subtract number of times called outside of interupt
  int Calc = (water::NbTopsFan * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/min
  Serial.print (Calc,DEC); //Prints the number calculated above
  Serial.print (" L/min\r\t"); //Prints "L/hour" and returns a new line
  Serial.print ("NbTopsFan:");
  Serial.print (water::NbTopsFan, DEC);
  Serial.print ("\n");
  return Calc;
}
