/*
 * Used pins: D22 D21 D23 D5 D18 D19 
 * Unused pins: 
 */

#include <Arduino.h>
#include "watch.h"
#include "memory.h"
#include "sensor.h"

watch rtc(false);
String data;
String timeStamp;
String message;
sensor simSen(0,dummy);

void setup()
{

  Serial.begin(57600);
  Serial.println("Hello Sustaingineering!");
  
  Serial.println("***********Initializing RTC...***********");
  rtc.initialize();
  Serial.println("*****************************************");
  
  Serial.println("*******Initializing MicroSD Card...******");
  memory::sdInitialize();
  Serial.println("*****************************************");
  
  memory::writeFile("/logs.txt", "Hello, this is Sustaingineering!\n");

  simSen.initialize();
  
  Serial.println("Setup Done!");
  
}

void loop()
{

  data = String("Data = ") + String(simSen.read());
  timeStamp = rtc.getTime();
  message = data + String("\t") + timeStamp + String("\n");
  memory::appendFile("/logs.txt", message);
  delay(1000);
  
}