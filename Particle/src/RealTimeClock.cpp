#include "RealTimeClock.h"

class RealTimeClock::Impl
{
public:
    Impl(bool need2SetTime);
    void initialize();
    String getTimeStamp();
    String getDate();
};

RealTimeClock::Impl::Impl(bool /*need2SetTime*/)
{}

void RealTimeClock::Impl::initialize()
{
    Time.zone(-7.0);
}

String RealTimeClock::Impl::getTimeStamp()
{
    return String((int)Time.now());
}

String RealTimeClock::Impl::getDate()
{
    return Time.format(Time.now(), "%F");
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
