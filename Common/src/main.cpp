#ifndef UNIT_TEST         // This macro is defined in PlatformIO toolchain and it is needed for ESP32.
#include "main.h"

void setup()
{
  bool success = true;
  bool status;
  
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  delay(1000);

  Serial.begin(115200);
  
  rtc.initialize(1604177282);

  bool memoryInitialized = memory.initialize();
  success = success && memoryInitialized;

  PersistentLogger::initialize(&memory, &rtc);

  // FIXME: do it in persistent data class
  if (memoryInitialized)
  {
    pumpIdInit();
  }

  LOGGER("Hello Sustaingineering!");

  status = hall_effect.initialize();
  success = success && status;

  status = volt_divider.initialize();
  success = success && status;

  status = thermocouple.initialize();
  success = success && status;

  status = waterflow.initialize();
  success = success && status;

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
  memory.appendFile(("/" + rtc.getDate() + ".txt").c_str(), (message + "\n").c_str());

#ifdef PARTICLE_H
  gsm.Publish(String(pumpId), message);
#endif
}
#endif // UNIT_TEST
