#include "Logger.h"

static SdCard *s_pSdCard;
static RealTimeClock *s_pRtc;

void initLogger(SdCard *pSdCard, RealTimeClock *pRtc)
{
    s_pSdCard = pSdCard;
    s_pRtc = pRtc;
}

void logger(bool condition, String file, String function, int line, String message)
{
    String time = s_pRtc->getTimeStamp();
    String toLog = "[" + time + "]" + "(" + file + ":" + function + ":" + line + ") " + message;
    
    if (condition)
    {
        Serial.println(toLog);
    }
    
    s_pSdCard->appendFile(("/" + s_pRtc->getDate() + ".log").c_str(), toLog.c_str(), true);
}