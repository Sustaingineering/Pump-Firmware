/*
 * Content: class that controls generic sensors, analog, digital and simulated sensor
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>

enum sensorType { digital, analog };

struct packet
{
    char type; //'r': request packet, 'v': voltage, 'i': current, 't': temperature, 'f': flow, 'c': count.
    float data;
};

//template <class T>
class farmSensor 
{
private:
    sensorType m_type;
protected:
    int m_pin;
    String m_name;
    String m_unit;
    char m_shortcut;
    float m_data;
    virtual float readRaw();
public:
    farmSensor(){}
    farmSensor(int pin, sensorType type, String name, String unit, char shortcut);
    virtual void initialize();
    String read();
    packet pack();
};

// Sensors to be used:
//  Voltage (V)
//  Current (A)
//  Water Flow ()
//  Temperature (C)