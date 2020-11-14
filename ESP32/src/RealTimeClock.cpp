#include "RealTimeClock.h"
#include <string.h>
#include <string>
#include <sys/time.h>
#include <time.h>

class RealTimeClock::Impl
{
public:
    Impl();
    void initialize(long initialTime);
    String getTimeStamp();
    String getDate();
};

RealTimeClock::Impl::Impl()
{
}

void RealTimeClock::Impl::initialize(long initialTime)
{
    timeval epoch = {(time_t) initialTime, 0};
    const timeval *tv = &epoch;
    settimeofday(tv, NULL);
}

String RealTimeClock::Impl::getTimeStamp()
{
    return String(time(NULL));
}

String RealTimeClock::Impl::getDate()
{
    // Return YYYY-MM-DD
    struct tm localtime;
    getLocalTime(&localtime);
    char buf[11]; 
    strftime(buf, sizeof(buf), "%G-%m-%d", &localtime);
    return String(buf);
}

void RealTimeClock::initialize(long initialTime) 
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
