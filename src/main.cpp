/*
 * Used pins: D22 D21 D23 D5 D18 D19 D4
 * Unused pins: 
 */

#include <Arduino.h>
#include "watch.h"
#include "memory.h"
#include "sensor.h"
#include "temp.h"

watch rtc(false);
String message;
//temp thermocouple(4);
sensor counter(0, soft, "Counter", "T");

void setup()
{
  pinMode(2,OUTPUT);
  Serial.begin(57600);
  Serial.println("\nHello Sustaingineering!\n");
  
  delay(1000);
  Serial.println("***********Initializing RTC**************");
  rtc.initialize();
  Serial.println("*****************************************\n");
  
  delay(1000);
  Serial.println("*******Initializing MicroSD Card*********");
  memory::sdInitialize();
  Serial.println("*****************************************\n");

  // delay(1000);
  // Serial.println("*******Initializing Thermocouple*********");
  // thermocouple.initialize();
  // Serial.println("*****************************************\n");
  
  //memory::writeFile("/logs.txt", "Hello, this is Sustaingineering!\n");

  counter.initialize();
  
  Serial.println("Setup Done!\n");
}

void loop()
{
  message = counter.read() + String("\t") + 
            rtc.getTimeStamp() + String("\n");
  Serial.print(message);
  memory::appendFile("/logs.txt", message);
  Serial.println(); delay(900); digitalWrite(2,HIGH); delay(100); digitalWrite(2,LOW);
}