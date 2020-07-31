
/*
 * Content: class that measures voltage using voltage dividers
 * Authors : Forbes Choy, Timothy Nguyen
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>
#include <farmSensor.h>

/* ------------------------- Tim Testing Inheritance ------------------------ */

class volt : public farmSensor {

    private:
    int m_RH; // voltage divider, higher resistor value [ohms]
    int m_RL; // voltage divider, lower resistor value [ohms]

    public:
        static const int adc_res = 4095; // ESP32 ADC resolution is 12 bits
        volt(int pin, sensorType type, String name, String unit, char shortcut, int RL, int RH);
        void initialize();
        float readRaw(); // overriding 4 functions from farmSensor class
        String read();  
        packet pack();
        
};

/* constructor */
volt::volt(int pin, sensorType type, String name, String unit, char shortcut, int RL, int RH): 
            farmSensor(pin, type, name, unit, shortcut), m_RH(RH), m_RL(RL) {}

/* overriding */
void volt::initialize()
{
    pinMode(m_pin, INPUT);
}


/* overriding */
float volt::readRaw(){
    return analogRead(m_pin) * (3.9 / adc_res) * ((m_RH + m_RL) / m_RL);
}

/* overriding */
String volt::read()
{
    m_data = readRaw(); // voltage read
    return String("Voltage") + String(": ") + String(m_data) + String(" (") + String("V") + String(")") + String(" | ");
}

/* overriding */
packet volt::pack()
{
    packet ret;
    ret.type = 'v';
    ret.data = m_data;
    return ret;
}




/* ------------------------- Original Code From Forbes ------------------------ */

/**
class volt
private:
    int pin; //analog pin number (the GPIO pin number) for data reading 
    int RH; //voltage divider, higher resistor value [ohms]
    int RL; //voltage divider, lower resistor value [ohms]
    float m_data; //changed to float instead of integer
    float readRaw(); //returns voltage of source (NOT THE DIVIDER) in decimal value

public:
    //constants declarations (cannot declare static const floats in .h files, need .cpp --> 3.9 defined in readRaw())
    static const int adc_res = 4095; //ESP32 ADC resolution is 12 bits

    //functions
    volt(int pin, int RL, int RH);
    void initialize();
    packet pack(); //for sending packets
    String read();
};

volt::volt(int _pin, int _RL, int _RH)
{
    pin = _pin;
    RL = _RL;
    RH = _RH; 
}

void volt::initialize()
{
    pinMode(pin, INPUT);
}

float volt::readRaw()
{
  //cannot define static const float values in header files --> needs to be in the .cpp file
  return analogRead(pin) * (3.9 / adc_res) * ((RH + RL) / RL); 
}

packet volt::pack()
{
    packet ret;
    ret.type = 'v';
    ret.data = m_data;
    return ret;
}

String volt::read()
{
    m_data = readRaw(); //voltage read
    return String("Voltage") + String(": ") + String(m_data) + String(" (") + String("V") + String(")") + String(" | ");
}
*/
