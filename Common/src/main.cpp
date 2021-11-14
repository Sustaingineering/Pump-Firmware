#ifndef UNIT_TEST         // This macro is defined in PlatformIO toolchain and it is needed for ESP32.
#include "main.h"

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

  //Writing on Sd Card
  memory.appendFile(("/" + rtc.getDate() + ".txt").c_str(), message.c_str());

#ifdef PARTICLE_H
  gsm.Publish(String(pumpId), message);
#endif

  Serial.println();
}
#endif // UNIT_TEST
