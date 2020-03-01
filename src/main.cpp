/*
  ___________________
o|EN              D23|*
o|VP(36)          D22|*
o|VN(39)       (1)TX0|x
o|D34          (3)RX0|x
o|D35             D21|*
o|D32   [ESP32]   D19|*
o|D33             D18|*
o|D25              D5|*
o|D26         (17)TX2|*
o|D27         (16)RX2|*
o|D14              D4|*
o|D12              D2|x
o|D13             D15|o
o|GND             GND|*
o|VIN_____________3V3|*

(o): unused.
(*): used.
(x): cannot be used.
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