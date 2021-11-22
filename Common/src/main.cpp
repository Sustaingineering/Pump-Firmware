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

  LOGGER("Hello Sustaingineering!");
  
  bool success = true;
  bool status;

  rtc.initialize(1604177282);

  status = hall_effect.initialize();
  success = success && status;

  status = volt_divider.initialize();
  success = success && status;

  status = thermocouple.initialize();
  success = success && status;

  status = waterflow.initialize();
  success = success && status;

  bool memoryInitialized = memory.initialize();
  success = success && memoryInitialized;
  // FIXME: do it in persistent data class
  if (memoryInitialized)
  {
    pumpIdInit();
    memory.getFreeSpace();
  }

#ifdef PARTICLE_H
  status = gsm.initialize();
  success = success && status;
#endif

  digitalWrite(BUILTIN_LED, !success);
  LOGGER("Setup Done!");

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
  LOGGER(message);

  //Writing on Sd Card
  memory.appendFile(("/" + rtc.getDate() + ".txt").c_str(), message.c_str());

#ifdef PARTICLE_H
  gsm.Publish(String(pumpId), message);
#endif
}
#endif // UNIT_TEST
