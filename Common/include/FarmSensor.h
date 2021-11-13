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


class FarmSensor
{
private:
    sensorType m_type;
protected:
    int m_pin;
    String m_name;
    String m_unit;
    char m_shortcut;
    float m_data;
    bool isWorking;
    virtual float readRaw();
public:
    FarmSensor();
    FarmSensor(int pin, sensorType type, String name, String unit, char shortcut);
    FarmSensor(int pin, String name, String unit, char shortcut);
    virtual bool initialize();
    String read();
    packet pack();
};

// Sensors to be used:
//  Voltage (V)
//  Current (A)
//  Water Flow ()
//  Temperature (C)