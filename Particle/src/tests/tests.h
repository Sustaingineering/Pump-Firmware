#include "PinConfig.h"
#include <Arduino.h>
#include <string.h>

#define TEST_ASSERT_NULL(ptr) ptr == NULL ? 1 : 0
#define TEST_FAIL_MESSAGE(msg) Serial.printf("TEST FAILED: %s\n", msg)
#define TEST_ASSERT_EQUAL_STRING(str1, str2) strcmp(str1, str2) == 0 ? true : false


#if EN_GSM
#include "Gsm.h"
extern Gsm gsm;
#endif

#if CURRENT
#include "Current.h"
extern Current hall_effect;
#endif

#if VOLTAGE
#include "Voltage.h"
extern Voltage volt_divider;
#endif

#if TEMPERATURE
#include "Temperature.h"
extern Temperature thermocouple; //pretty slow response and depends greatly on the surface temperature of the thermocouple tip
#endif

#if FLOW
#include "Flow.h"
extern Flow waterflow;
#endif

#include "RealTimeClock.h"
extern RealTimeClock rtc;

#if SDCARD
#include "SdCard.h"
extern SdCard memory;
#endif

void tests(); 
void printTestStart(String tag);
void printTestEnd(String tag);
