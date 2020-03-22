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
#include "memory.h"
#include "farmSensor.h"
#include "temp.h"
#include "LoRaTransceiver.h"

int counterData;
// Sensors data holders go here.
String message;

watch rtc(false);
LoRaTransceiver receiver(15, 27, 26, 0xF3);
farmSensor counter(0, soft, "Counter", "T");
//temp thermocouple(4);
// Sensors Constructors go here.

void setup()
{
  pinMode(2,OUTPUT);
  Serial.begin(57600);
  Serial.println("\nHello Sustaingineering!\n");
  
  Serial.println("Initializing RTC...");
  rtc.initialize();
  Serial.println("RTC Initialized.\n");
  
  Serial.println("Initializing MicroSD Card...");
  memory::sdInitialize();
  Serial.println("MicroSD Card Initialized.\n");

  Serial.println("Initializing LoRa...");
  receiver.initialize();
  Serial.println("LoRa Initialized.\n");

  Serial.println("Initializing Counter...");
  counter.initialize();
  Serial.println("Counter Initialized.");

  //Serial.println("Initializing Thermocouple...");
  //thermocouple.initialize();
  //Serial.println("Thermocouple Initialized\n");

  // Sensors Initializers go here.
  
  Serial.println("Setup Done!\n");
}

void loop()
{
  message = counter.read(&counterData) + String("\t");
  message += rtc.getTimeStamp() + String("\n");
  Serial.print(message);
  memory::appendFile("/logs.txt", message);
  digitalWrite(BUILTIN_LED, LOW);
  receiver.respond({'c', counterData});
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.println();
}