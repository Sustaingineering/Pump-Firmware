#include "RealTimeClock.h"
#include <string.h>
#include <string>
class RealTimeClock::Impl
{
public:
    Impl();
    void initialize(time_t initialTime);
    String getTimeStamp();
    String getDate();
};

RealTimeClock::Impl::Impl()
{
}

void RealTimeClock::Impl::initialize(time_t initialTime)
{
    timeval epoch = {initialTime, 0};
    const timeval *tv = &epoch;
    settimeofday(tv, NULL);
}

String RealTimeClock::Impl::getTimeStamp()
{
    return String(time(NULL));
}

String RealTimeClock::Impl::getDate()
{
    // Return "Year, Month, Date"
    struct tm localtime;
    getLocalTime(&localtime);
    char buf[40];
    strftime(buf, sizeof(buf), "%A, %B %d %Y %H:%M:%S", &localtime);
    return String(buf);
}

void RealTimeClock::initialize(time_t initialTime)
{
    m_pImpl->initialize(initialTime);
}

String RealTimeClock::getTimeStamp()
{
    return m_pImpl->getTimeStamp();
}

String RealTimeClock::getDate()
{
    return m_pImpl->getDate();
}
