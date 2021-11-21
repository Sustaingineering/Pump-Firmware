/*
 * Content: Logger class
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once

#include <Arduino.h>
#include "SdCard.h"
#include "RealTimeClock.h"
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define SDLOG(message) sdLog(LOG_MODULE_SWITCH, __FILENAME__, __FUNCTION__, __LINE__, message)

#define LOG_CURRENT_SWITCH      1
#define LOG_SENSOR_SWITCH       1
#define LOG_FLOW_SWITCH         1
#define LOG_GSM_SWITCH          1
#define LOG_MAIN_SWITCH         1
#define LOG_RTC_SWITCH          1
#define LOG_SDCARD_SWITCH       1
#define LOG_TEMPERATURE_SWITCH  1
#define LOG_VOLTAGE_SWITCH      1

void initLogger(SdCard *pSdCard, RealTimeClock *pRtc);
void sdLog(bool condition, String file, String function, int line, String message);
