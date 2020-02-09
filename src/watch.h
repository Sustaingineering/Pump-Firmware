/*
 * RTC -> ESP32 
 * VCC -> 3V3
 * GND -> GND
 * SCL -> D22
 * SDA -> D21
 */

#include <Arduino.h>
#include "RTClib.h"

class watch
{
private:
    RTC_DS1307 m_rtc;
    bool m_need2SetTime;
public:
    watch(bool need2SetTime);
    void initialize();
    String  getTime();
};

watch::watch(bool need2SetTime): m_rtc()
{
    m_need2SetTime = need2SetTime;
}

void watch::initialize()
{
    m_rtc.begin();
    if (! m_rtc.isrunning())
        Serial.println("RTC is NOT running!");
    if (m_need2SetTime)
        m_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // January 21, 2014 at 3am you would call:
    // m_rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

String watch::getTime()
{
    if (m_rtc.isrunning()) {
        DateTime time = m_rtc.now();
        return String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL);
    }
    else
    {
        return String("m_rtc is not running");
    }
}