
/*
 * Content: class that measures current using the ACS711EX Hall Effect Sensor
 * Author : Forbes Choy
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include <farmSensor.h>


class current
{
private:
    int pin; //analog pin number (the GPIO pin number) for data reading
    float m_data; //changed to float instead of integer
    float readRaw(); //returns currentage of source (NOT THE DIVIDER) in decimal value

public:
    //constants declarations (cannot declare static const floats in .h files, need .cpp --> 3.9 defined in readRaw())
    static const int adc_res = 4095; //ESP32 ADC resolution is 12 bits

    //functions
    current(int pin);
    void initialize();
    packet pack(); //for sending packets
    String read();
};

current::current(int _pin)
{
    pin = _pin; 
}

void current::initialize()
{
    pinMode(pin, INPUT);
}

float current::readRaw()
{
  int x = analogRead(pin); // Take reading
  // Convert to decimal
  float HallVoltage = x * (3.9 / adc_res);
  // Compute the current from the voltage reading equation
  float HallAmps = (HallVoltage * 22.0) / 3.0 - (55.0 / 3.0);


  //cannot define static const float values in header files --> needs to be in the .cpp file
//   return analogRead(pin) * (3.9 / adc_res) * ((RH + RL) / RL); 
  return HallAmps;
}

packet current::pack()
{
    packet ret;
    ret.type = 'i';
    ret.data = m_data;
    return ret;
}

String current::read()
{
    m_data = readRaw(); //currentage read
    return String("Currrent") + String(": ") + String(m_data) + String(" (") + String("A") + String(")") + String(" | ");
}