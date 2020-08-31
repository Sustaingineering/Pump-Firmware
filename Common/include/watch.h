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
#include <RTClib.h>

class watch
{
private:
    RTC_DS1307 m_rtc;
    bool m_need2SetTime;
public:
    watch(bool need2SetTime);
    void initialize();
    String getTimeStamp();
    String getDate();
};
