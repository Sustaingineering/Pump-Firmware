#include "PinConfig.h"
#include <Arduino.h>

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

#if ERTC
#include "RealTimeClock.h"
extern RealTimeClock rtc;
#endif

#if SDCARD
#include "SdCard.h"
extern SdCard memory;
#endif

void tests(); 
void printTestStart(String tag);
void printTestEnd(String tag);