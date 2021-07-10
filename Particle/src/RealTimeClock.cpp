#include "RealTimeClock.h"

class RealTimeClock::Impl
{
public:
    Impl();
    void initialize();
    String getTimeStamp();
    String getDate();
};

RealTimeClock::Impl::Impl()
{}

void RealTimeClock::Impl::initialize()
{
    Serial.println("Initializing RTC...");
    Time.zone(-7.0);
    Serial.println("Initializing RTC Done.");
}

String RealTimeClock::Impl::getTimeStamp()
{
    return String((int)Time.now());
}

String RealTimeClock::Impl::getDate()
{
    return Time.format(Time.now(), "%F");
}

void RealTimeClock::initialize(long initialTime)
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
