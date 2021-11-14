#ifndef UNIT_TEST         // This macro is defined in PlatformIO toolchain and it is needed for ESP32.
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
#include "Counter.h"

int pumpId = 0;         // FIXME: do it in persistent data class

#ifdef PARTICLE_H
#if EN_GSM
SYSTEM_MODE(AUTOMATIC)
Gsm gsm;
#else
SYSTEM_MODE(MANUAL)
#endif
#endif

//Global Objects
String message;

RealTimeClock rtc;

#if CURRENT
Current hall_effect(CURRENT_PIN, MAX_V);
#endif

#if VOLTAGE
Voltage volt_divider(VOLT_PIN, 25000, 1000000, MAX_V);
#endif

#if TEMPERATURE
Temperature thermocouple(TEMP_PIN); //pretty slow response and depends greatly on the surface temperature of the thermocouple tip
#endif

#if FLOW
Flow waterflow(FLOW_PIN);
#endif

#if SDCARD
SdCard memory(SDCARD_SELECT_PIN);
// FIXME: do it in persistent data class
void pumpIdInit()
{
  char *idBuf = memory.readFile("/pump-id.txt");
  if (idBuf != NULL)
  {
    pumpId = strtol(idBuf, NULL, 10);
    free(idBuf);
    Serial.printf("PumpID is: %d\n", pumpId);
  }
}
#endif

#if COUNTERS
Counter counters[COUNTERS];
#endif

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.println("\nHello Sustaingineering!\n");
  delay(1000);
  bool success = true;

  rtc.initialize(1604177282);

#if CURRENT
  success = success && hall_effect.initialize();
#endif

#if VOLTAGE
  success = success && volt_divider.initialize();
#endif

#if TEMPERATURE
  success = success && thermocouple.initialize();
#endif

#if FLOW
  success = success && waterflow.initialize();
#endif

#if SDCARD
  bool memoryInitialized = memory.initialize();
  success = success && memoryInitialized;
  // FIXME: do it in persistent data class
  if (memoryInitialized)
  {
    pumpIdInit();
    memory.getFreeSpace();
  } 
#endif

#if COUNTERS
    for (int i = 0; i < COUNTERS; i++)
    counters[i].initialize();
#endif

#ifdef PARTICLE_H
#if EN_GSM
  success = success && gsm.initialize();
#endif
#endif

  Serial.println("Setup Done!\n");
  digitalWrite(BUILTIN_LED, !success);

#if PARTICLE_UNIT_TESTS
  tests();
#endif
}

void loop()
{
#if PARTICLE_UNIT_TESTS
  return;
#endif

  message = "";
  //Sampling Sensors
#if COUNTERS
  for (int i = 0; i < COUNTERS; i++)
    message += counters[i].read();
#endif

#if CURRENT
  message += hall_effect.read();
#endif

#if VOLTAGE
  message += volt_divider.read();
#endif

#if TEMPERATURE
  message += thermocouple.read();
#endif

#if FLOW
  message += waterflow.read();
#else
  delay(1000);
#endif

  message += rtc.getTimeStamp();
  Serial.println(message);

#if SDCARD
  //Writing on Sd Card
  memory.appendFile(("/" + rtc.getDate() + ".txt").c_str(), message.c_str());
#endif

#ifdef PARTICLE_H
#if EN_GSM
  gsm.Publish(String(pumpId), message);
#endif
#endif

  Serial.println();
}
#endif // UNIT_TEST
