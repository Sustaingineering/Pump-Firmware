#ifndef UNIT_TEST         // This macro is defined in PlatformIO toolchain and it is needed for ESP32.
#include "main.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_MAIN_SWITCH

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  delay(1000);

  Serial.begin(115200);

  initLogger(&memory, &rtc);

  SDLOG("Hello Sustaingineering!");
  
  bool success = true;

  rtc.initialize(1604177282);

  success = success && hall_effect.initialize();

  success = success && volt_divider.initialize();

  success = success && thermocouple.initialize();

  success = success && waterflow.initialize();

  bool memoryInitialized = memory.initialize();
  success = success && memoryInitialized;
  // FIXME: do it in persistent data class
  if (memoryInitialized)
  {
    pumpIdInit();
    memory.getFreeSpace();
  }

#ifdef PARTICLE_H
  success = success && gsm.initialize();
#endif

  digitalWrite(BUILTIN_LED, !success);
  SDLOG("Setup Done!");

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
  SDLOG(message);

  //Writing on Sd Card
  memory.appendFile(("/" + rtc.getDate() + ".txt").c_str(), message.c_str());

#ifdef PARTICLE_H
  gsm.Publish(String(pumpId), message);
#endif
}
#endif // UNIT_TEST
