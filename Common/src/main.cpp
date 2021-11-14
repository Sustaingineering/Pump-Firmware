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

#if SDCARD_SWITCH
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

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.println("\nHello Sustaingineering!\n");
  delay(1000);
  bool success = true;

  rtc.initialize(1604177282);

  success = success && hall_effect.initialize();

  success = success && volt_divider.initialize();

  success = success && thermocouple.initialize();

  success = success && waterflow.initialize();

#if SDCARD_SWITCH
  bool memoryInitialized = memory.initialize();
  success = success && memoryInitialized;
  // FIXME: do it in persistent data class
  if (memoryInitialized)
  {
    pumpIdInit();
    memory.getFreeSpace();
  } 
#endif

#ifdef PARTICLE_H
  success = success && gsm.initialize();
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

  message += hall_effect.read();

  message += volt_divider.read();

  message += thermocouple.read();

  message += waterflow.read();

  message += rtc.getTimeStamp();
  Serial.println(message);

#if SDCARD_SWITCH
  //Writing on Sd Card
  memory.appendFile(("/" + rtc.getDate() + ".txt").c_str(), message.c_str());
#endif

#ifdef PARTICLE_H
  gsm.Publish(String(pumpId), message);
#endif

  Serial.println();
}
#endif // UNIT_TEST
