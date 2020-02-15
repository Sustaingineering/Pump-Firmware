/*
 * Used pins: D22 D21 D23 D5 D18 D19 
 * Unused pins: 
 */

#include <Arduino.h>
#include "watch.h"
#include "memory.h"
#include "sensor.h"
#include "Temp.h"

watch rtc(false);
String data;
String timeStamp;
String message;
temp thermocouple(4);

//sensor simSen(0,dummy);

void setup()
{

  Serial.begin(57600);
  Serial.println("Hello Sustaingineering!");
  
  delay(1000);
  Serial.println("***********Initializing RTC...***********");
  rtc.initialize();
  Serial.println("*****************************************");
  
  delay(1000);
  Serial.println("*******Initializing MicroSD Card...******");
  memory::sdInitialize();
  Serial.println("*****************************************");

  delay(1000);
  Serial.println("*******Initializing Thermocouple...******");
  thermocouple.initialize();
  Serial.println("*****************************************");
  
  memory::writeFile("/logs.txt", "Hello, this is Sustaingineering!\n");

  //simSen.initialize();
  
  Serial.println("Setup Done!");
  
}

void loop()
{
  data = String("Temp = ") + String(thermocouple.readRaw());
  timeStamp = rtc.getTime();
  message = data + String("\t") + timeStamp + String("\n");
  Serial.print(message);
  memory::appendFile("/logs.txt", message);
  //memory::readFile("/logs.txt");
  delay(1000);
  
}