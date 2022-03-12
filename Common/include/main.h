#pragma once

#include <Arduino.h>
#include "PinConfig.h"
#include "RealTimeClock.h"
#include "SdCard.h"
#include "FarmSensor.h"
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "Flow.h"
#ifdef PARTICLE_H
#include "Gsm.h"
#endif
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_MAIN_SWITCH

int pumpId = 0;         // FIXME: do it in persistent data class

#ifdef PARTICLE_H
Gsm gsm(GSM_SWITCH);
#endif

//Global Objects
String message;

RealTimeClock rtc;

Current hall_effect(CURRENT_SWITCH, CURRENT_PIN, MAX_V);

Voltage volt_divider(VOLTAGE_SWITCH, VOLT_PIN, 25000, 1000000, MAX_V);

Temperature thermocouple(TEMPERATURE_SWITCH, TEMP_PIN); //pretty slow response and depends greatly on the surface temperature of the thermocouple tip

Flow waterflow(FLOW_SWITCH, FLOW_PIN);

SdCard memory(SDCARD_SWITCH, SDCARD_SELECT_PIN);
// FIXME: do it in persistent data class
void pumpIdInit()
{
  char *idBuf = memory.readFile("/pump-id.txt");
  if (idBuf != NULL)
  {
    pumpId = strtol(idBuf, NULL, 10);
    free(idBuf);
    LOGGER("PumpID is: " +  String(pumpId));
  }
}