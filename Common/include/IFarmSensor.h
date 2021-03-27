/*
 * Content: Interface for all sensors
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

class IFarmSensor 
{
public:
    virtual bool initialize() = 0;
    virtual String read() = 0;
    virtual packet pack() = 0;
};
