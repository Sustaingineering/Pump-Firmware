/*
            ___________________
          o|EN              D23|* VSPI MOSI
          o|VP(36)          D22|*
          o|VN(39)       (1)TX0|x
          o|D34          (3)RX0|x
          o|D35             D21|*
          o|D32   [ESP32]   D19|* VSPI MISO
          o|D33             D18|* VSPI CLK
          o|D25              D5|* VSPI CS
          *|D26         (17)TX2|*
          *|D27         (16)RX2|*
HSPI CLK  *|D14              D4|*
HSPI MISO *|D12              D2|*
HSPI MOSI *|D13             D15|* HSPI CS
          o|GND             GND|*
          o|VIN_____________3V3|*

(o): unused.
(*): used.
(x): cannot be used.
*/

#include <Arduino.h>
#include "watch.h"
#include "memory.h"
#include "farmSensor.h"
#include "temp.h"
#include "LoRaTransceiver.h"

watch rtc(false);
String message;
//temp thermocouple(4);
farmSensor counter(0, soft, "Counter", "T");
LoRaTransceiver receiver(15, 27, 26, 0xF3);

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

  counter.initialize();

  delay(1000);
  Serial.println("**********Initializing LoRa**************");
  receiver.initialize();
  Serial.println("*****************************************\n");
  
  Serial.println("Setup Done!\n");
}

void loop()
{
  message = counter.read() + String("\t") + 
            rtc.getTimeStamp() + String("\n");
  Serial.print(message);
  memory::appendFile("/logs.txt", message);
  receiver.send({'c', counter.readRaw()});

  Serial.println(); delay(900); digitalWrite(2,HIGH); delay(100); digitalWrite(2,LOW);
}