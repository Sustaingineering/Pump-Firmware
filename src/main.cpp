/*
            ___________________
          ?|EN              D23|* VSPI MOSI
          ?|VP(D36)         D22|* I2C SCL
          ?|VN(D39)     (D1)TX0|x
          o|D34         (D3)RX0|x
          o|D35             D21|* I2C SDA
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
#include "Restarter.h"
#include "watch.h"
#include "SdCard.h"
#include "farmSensor.h"
#include "temp.h"
#include "LoRaTransceiver.h"

//Global Variables
bool LoRaStatus;
String message;
packet packets[NUMBER_OF_PACKETS];

//Global Objects
Restarter restarter(5);
watch rtc(false);
SdCard memory;
LoRaTransceiver receiver(15, 27, 26, 0xF3);
farmSensor counter1(0, counter, "Counter1", "T", 'c');
farmSensor counter2(0, counter, "Counter2", "T", 'd');
farmSensor counter3(0, counter, "Counter3", "T", 'e');
farmSensor counter4(0, counter, "Counter4", "T", 'f');
farmSensor counter5(0, counter, "Counter4", "T", 'g');
farmSensor counter6(0, counter, "Counter4", "T", 'h');
//temp thermocouple(4);
//Sensors Constructors go here.

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
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
  message  = counter1.read();
  message += counter2.read();
  message += counter3.read();
  message += counter4.read();
  message += counter5.read();
  message += counter6.read();
  message += rtc.getTimeStamp() + String("\n");
  
  //Writing on Sd Card
  Serial.print(message);
  memory.appendFile("/logs.txt", message);
  digitalWrite(BUILTIN_LED, LOW);
  
  //Responding to a request from LoRa
  packets[0] = counter1.pack();
  packets[1] = counter2.pack();
  packets[2] = counter3.pack();
  packets[3] = counter4.pack();
  packets[4] = counter5.pack();
  packets[5] = counter6.pack();
  LoRaStatus = receiver.respond(packets, NUMBER_OF_PACKETS);
  restarter.takeAction(LoRaStatus);
  Serial.println();
}