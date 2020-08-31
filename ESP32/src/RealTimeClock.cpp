#include "RealTimeClock.h"
#include <RTClib.h>

class RealTimeClock::Impl
{
private:
    RTC_DS1307 m_rtc;
    bool m_need2SetTime;
public:
    Impl(bool need2SetTime);
    void initialize();
    String getTimeStamp();
    String getDate();
};

RealTimeClock::Impl::Impl(bool need2SetTime): m_rtc()
{
    m_need2SetTime = need2SetTime;
}

void RealTimeClock::Impl::initialize()
{
    m_rtc.begin();
    if (! m_rtc.isrunning())
        Serial.println("RTC is NOT running!");
    if (m_need2SetTime)
        m_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.print("Current Time: ");
    Serial.print(m_rtc.now().timestamp(DateTime::TIMESTAMP_FULL));
    Serial.println();
    // January 21, 2014 at 3am you would call:
    // m_rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

}

String RealTimeClock::Impl::getTimeStamp()
{
    if (m_rtc.isrunning()) {
        DateTime time = m_rtc.now();
        return time.timestamp(DateTime::TIMESTAMP_FULL);
    }
    else
    {
        return String("m_rtc is not running");
    }
}

String RealTimeClock::Impl::getDate()
{
    if (m_rtc.isrunning())
    {
    DateTime time = m_rtc.now();
    return time.timestamp(DateTime::TIMESTAMP_DATE);
    }
    else
    {
        return String("m_rtc is not running");
    }
}

RealTimeClock::RealTimeClock(bool need2SetTime): m_pImpl(new RealTimeClock::Impl(need2SetTime))
{}

void RealTimeClock::initialize()
{
    m_pImpl->initialize();
}

String RealTimeClock::getTimeStamp()
{
    return m_pImpl->getTimeStamp();
}

String RealTimeClock::getDate()
{
    return m_pImpl->getDate();
}