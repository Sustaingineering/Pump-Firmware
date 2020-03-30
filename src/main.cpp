/*
            ___________________
          ?|EN              D23|* VSPI MOSI
          ?|VP(D36)         D22|*
          ?|VN(D39)     (D1)TX0|x
          o|D34         (D3)RX0|x
          o|D35             D21|*
          o|D32   [ESP32]   D19|* VSPI MISO
          o|D33             D18|* VSPI CLK
          o|D25              D5|* VSPI CS
          *|D26        (D17)TX2|*
          *|D27        (D16)RX2|*
HSPI CLK  *|D14              D4|*
HSPI MISO *|D12              D2|* BUILTIN_LED
HSPI MOSI *|D13             D15|* HSPI CS
          o|GND             GND|*
          o|VIN_____________3V3|*

(o): unused.
(*): used.
(x): cannot be used.
(?): WTF
*/

#include <Arduino.h>
#include "watch.h"
#include "SdCard.h"
#include "farmSensor.h"
#include "temp.h"
#include "LoRaTransceiver.h"

//Global Variables
int counterData1;
int counterData2;
int counterData3;
int counterData4;
// Sensors data holders go here.
String message;
packet packets[NUMBER_OF_PACKETS];

//Global Objects
watch rtc(false);
SdCard memory;
LoRaTransceiver receiver(15, 27, 26, 0xF3);
farmSensor counter1(0, soft, "Counter1", "T");
farmSensor counter2(0, soft, "Counter2", "T");
farmSensor counter3(0, soft, "Counter3", "T");
farmSensor counter4(0, soft, "Counter4", "T");
//temp thermocouple(4);
//Sensors Constructors go here.

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(57600);
  Serial.println("\nHello Sustaingineering!\n");
  
  Serial.println("Initializing RTC...");
  rtc.initialize();
  Serial.println("RTC Initialized.\n");
  
  Serial.println("Initializing MicroSD Card...");
  memory.initialize();
  Serial.println("MicroSD Card Initialized.\n");

  Serial.println("Initializing LoRa...");
  receiver.initialize();
  Serial.println("LoRa Initialized.\n");

  //Serial.println("Initializing Thermocouple...");
  //thermocouple.initialize();
  //Serial.println("Thermocouple Initialized\n");

  // Sensors Initializers go here.
  
  Serial.println("Setup Done!\n");
}

void loop()
{
  //Sampling Sensors
  digitalWrite(BUILTIN_LED, HIGH);
  message  = counter1.read(&counterData1);
  message += counter2.read(&counterData2);
  message += counter3.read(&counterData3);
  message += counter4.read(&counterData4);
  message += rtc.getTimeStamp() + String("\n");
  
  //Writing on Sd Card
  Serial.print(message);
  memory.appendFile("/logs.txt", message);
  digitalWrite(BUILTIN_LED, LOW);
  
  //Waiting for a request on LoRa
  packets[0].type = 'c'; packets[0].data = counterData1;
  packets[1].type = 'd'; packets[1].data = counterData2;
  packets[2].type = 'e'; packets[2].data = counterData3;
  packets[3].type = 'f'; packets[3].data = counterData4;
  receiver.respond(packets, NUMBER_OF_PACKETS);
  
  Serial.println();
}