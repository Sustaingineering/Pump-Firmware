/*
 * Content: class that controls RTC module
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

/*
 * RTC -> ESP32 
 * VCC -> 3V3
 * GND -> GND
 * SCL -> D22
 * SDA -> D21
 */

#pragma once
#include <Arduino.h>


class RealTimeClock
{
private:
    class Impl;
    Impl *m_pImpl;

public:
    void initialize(time_t initialTime);
    String getTimeStamp();
    String getDate();
};
