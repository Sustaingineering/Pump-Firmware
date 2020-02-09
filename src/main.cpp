/*
 * Used pins: D22 D21 D23 D5 D18 D19 
 * Unused pins: 
 */

#include <Arduino.h>
#include "watch.h"
#include "memory.h"

watch rtc(false);
String data;
String timeStamp;
String message;

void setup() {
  Serial.begin(57600);
  Serial.println("Hello Sustaingineering!");
  
  Serial.println("***********Initializing RTC...***********");
  rtc.initialize();
  Serial.println("*****************************************");
  
  Serial.println("*******Initializing MicroSD Card...******");
  memory::sdInitialize();
  Serial.println("*****************************************");
  
  memory::writeFile("/logs.txt", "Hello, this is Sustaingineering!");
  
  Serial.println("Setup Done!");
}

void loop() {
  data = String("Some Data = 4738756473826783");
  timeStamp = rtc.getTime();
  message = data + String("\t") + timeStamp + String("\n");
  memory::appendFile("/logs.txt", message);
  delay(1000);
}