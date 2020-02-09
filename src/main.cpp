/*
 * Used pins: D22 D21 D23 D5 D18 D19 
 * Unused pins: 
 */

#include <Arduino.h>
#include "watch.h"
#include "memory.h"

watch rtc(false);

void setup() {
  Serial.begin(57600);
  Serial.println("Hello Sustaingineering!");
  
  Serial.println("***********Initializing RTC...***********");
  rtc.initialize();
  Serial.println("*****************************************");
  
  Serial.println("*******Initializing MicroSD Card...******");
  memory::sdInitialize();
  Serial.println("*****************************************");
  
  Serial.println("Setup Done!");
}

void loop() {
  
}