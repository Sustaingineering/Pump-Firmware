#include "Logger.h"

unsigned int PersistentLogger::m_linesLogged = 0;
SdCard* PersistentLogger::m_pSdCard = nullptr;
RealTimeClock* PersistentLogger::m_pRtc = nullptr;

void PersistentLogger::initialize(SdCard *pSdCard, RealTimeClock *pRtc)
{
    m_linesLogged = 0;
    m_pSdCard = pSdCard;
    m_pRtc = pRtc;
}

void PersistentLogger::log(bool condition, String file, String function, int line, String message)
{
    String time = m_pRtc->getTimeStamp();
    String toLog = "[" + time + "]" + "(" + file + ":" + function + ":" + line + ") " + message + "\n";
    
    if (condition)
    {
        Serial.print(toLog);
    }
    
    m_pSdCard->appendFile(("/" + m_pRtc->getDate() + ".log").c_str(), toLog.c_str(), true);
    m_linesLogged++;
}