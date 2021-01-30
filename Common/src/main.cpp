#include <Arduino.h>
#include "PinConfig.h"
//#include "Restarter.h"
#include "RealTimeClock.h"
#include "SdCard.h"
#include "farmSensor.h"
#include "volt.h"
#include "current.h"
#include "temp.h"
#ifndef electron
#include "Flow.h"
#include "LoRaTransceiver.h"
#else
#include "Gsm.h"
#endif
#include "counter.h"

int pumpId;

//Global Objects
//Restarter restarter(5);
String message;

#ifdef electron
Gsm gsm;
#endif

#if CURRENT
current hall_effect(CURRENT_PIN, "Current", "Amps", 'i', MAX_V);
#endif

#if VOLTAGE
volt volt_divider(VOLT_PIN, analog, "Voltage", "Volt", 'v', 25000, 1000000, MAX_V);
#endif

#if TEMP
temp thermocouple(TEMP_PIN, digital, "Temperature", "Celsius", 't'); //pretty slow response and depends greatly on the surface temperature of the thermocouple tip
#endif

#if FLOW
flow waterflow(FLOW_PIN,"WaterFlow", "L/min", 'f');
#endif

#if ERTC
RealTimeClock rtc;
#endif

#if SDCARD
SdCard memory(SDCARD_SELECT_PIN);
#endif

#if LORA
bool LoRaStatus;
packet packets[NUMBER_OF_PACKETS];
#if GSM
LoRaTransceiver requester(LORA_SELECT_PIN, LORA_RST_PIN, LORA_DIO0_PIN, LORA_SECRET_WORD);
#else
LoRaTransceiver responder(LORA_SELECT_PIN, LORA_RST_PIN, LORA_DIO0_PIN, LORA_SECRET_WORD, PUMP_ID);
#endif //GSM
#endif //LORA

#if COUNTERS
  counter **counterArray;
#endif

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nHello Sustaingineering!\n");

#ifdef electron
  gsm.initialize();
#endif
  
#if CURRENT
  Serial.println("Initializing Current Sensor...");
  hall_effect.initialize();
  Serial.println("Current Sensor Initialized.\n");
#endif

#if VOLTAGE
  Serial.println("Initializing Voltage Sensor...");
  volt_divider.initialize();
  Serial.println("Voltage Sensor Initialized.\n");
#endif
  
#if TEMP
  Serial.println("Initializing Thermocouple...");
  thermocouple.initialize();
  Serial.println("Thermocouple Initialized.\n");
#endif

#if FLOW
  Serial.println("Initializing Waterflow...");
  waterflow.initialize();
  Serial.println("WaterFlow Initialized.\n");
#endif

#if ERTC
  Serial.println("Initializing RTC...");
  rtc.initialize(1604177282); //Oct 31, 2020 - 1:48
  Serial.println("RTC Initialized.\n");
#endif

#if SDCARD
  Serial.println("Initializing MicroSD Card...");
  memory.initialize();
  Serial.println("MicroSD Card Initialized.\n");
  char *idBuf = memory.readFile("/pump-id.txt");
  if (idBuf != NULL)
  {
    pumpId = strtol(idBuf, NULL, 10);
    free(idBuf);
    Serial.printf("PumpID is: %d\n", pumpId);
  }
  else
  {
    Serial.printf("Pump ID Initialization failed");
  }
#endif

#if LORA
  Serial.println("Initializing LoRa...");
#if GSM
  requester.initialize();
#else
  responder.initialize();
#endif // GSM
  Serial.println("LoRa Initialized.\n");
#endif // LORA

#if COUNTERS
  counterArray = counter::createCounters(COUNTERS);
#endif

  //Sensors Initializers go here.

  Serial.println("Setup Done!\n");
}


void loop()
{
  message = "";
  //Sampling Sensors
#if COUNTERS
  for (int i = 0; i < COUNTERS; i++)
    message += counterArray[i]->read();
#endif

#if CURRENT
  message += hall_effect.read();
#endif

#if VOLTAGE
  message += volt_divider.read();
#endif

#if TEMP
  message += thermocouple.read();
#endif

#if FLOW
  message += waterflow.read();
#endif

#if ERTC
  message += rtc.getTimeStamp();
#endif
  message += String("\n");
  Serial.print(message);
#if SDCARD
  //Writing on Sd Card
  memory.appendFile(("/" + rtc.getDate() + ".txt").c_str(), message.c_str());
#endif

#if LORA
#ifdef electron 
  // make requests
  requester.request(0, packets, NUMBER_OF_PACKETS);
  Serial.print("received = [");
  for (int i = 0; i < NUMBER_OF_PACKETS; i++)
  {
    Serial.print(packets[i].type);
    Serial.print(packets[i].data);
    if (i != (NUMBER_OF_PACKETS - 1))
      Serial.print(", ");
  }
  Serial.print("]");
  delay(100);
#else //electron
  //Responding to a request from LoRa
  packets[0] = counter1.pack();
  packets[1] = counter2.pack();
  packets[2] = counter3.pack();
  packets[3] = counter4.pack();
  packets[4] = counter5.pack();
  packets[5] = counter6.pack();
  LoRaStatus = responder.respond(packets, NUMBER_OF_PACKETS);
#endif // electron
#else //LORA
  delay(1000);
#endif //LORA

#ifdef electron
  gsm.Publish(String(pumpId), message);
#endif

  //restarter.takeAction(LoRaStatus);
  Serial.println();
}
