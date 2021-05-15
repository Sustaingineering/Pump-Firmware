#ifndef UNIT_TEST
#include <Arduino.h>
#include "PinConfig.h"
//#include "Restarter.h"
#include "RealTimeClock.h"
#include "SdCard.h"
#include "FarmSensor.h"
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "Flow.h"
#ifndef PARTICLE_H
#include "LoRaTransceiver.h"
#else
#include "Gsm.h"
#endif
#include "Counter.h"

int pumpId = 0;

#ifdef PARTICLE_H
#if EN_GSM
SYSTEM_MODE(AUTOMATIC)
Gsm gsm;
#else
SYSTEM_MODE(MANUAL)
#endif
#endif

//Global Objects
//Restarter restarter(5);
String message;

#if CURRENT
Current hall_effect(CURRENT_PIN, "Current", "Amps", 'i', MAX_V);
#endif

#if VOLTAGE
Voltage volt_divider(VOLT_PIN, analog, "Voltage", "Volt", 'v', 25000, 1000000, MAX_V);
#endif

#if TEMPERATURE
Temperature thermocouple(TEMP_PIN, digital, "Temperature", "Celsius", 't'); //pretty slow response and depends greatly on the surface temperature of the thermocouple tip
#endif

#if FLOW
Flow waterflow(FLOW_PIN, "WaterFlow", "L/min", 'f');
#endif

#if ERTC
RealTimeClock rtc;
#endif

#if SDCARD
SdCard memory(SDCARD_SELECT_PIN);
void pumpIdInit()
{
  char *idBuf = memory.readFile("/pump-id.txt");
  if (idBuf != NULL)
  {
    pumpId = strtol(idBuf, NULL, 10);
    free(idBuf);
    Serial.printf("PumpID is: %d\n", pumpId);
  }
}
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
Counter **counterArray;
#endif

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nHello Sustaingineering!\n");
  bool success = true;

#if CURRENT
  digitalWrite(BUILTIN_LED,HIGH);
  success = success && hall_effect.initialize();
  delay(100);
  digitalWrite(BUILTIN_LED,LOW);
#endif

#if VOLTAGE
  digitalWrite(BUILTIN_LED,HIGH);
  success = success && volt_divider.initialize();
  delay(100);
  digitalWrite(BUILTIN_LED,LOW);
#endif

#if TEMPERATURE
  digitalWrite(BUILTIN_LED,HIGH);
  success = success && thermocouple.initialize();
  delay(100);
  digitalWrite(BUILTIN_LED,LOW);
#endif

#if FLOW
  digitalWrite(BUILTIN_LED,HIGH);
  success = success && waterflow.initialize();
  delay(100);
  digitalWrite(BUILTIN_LED,LOW);
#endif

#if ERTC
  Serial.println("Initializing RTC...");
  rtc.initialize(1604177282);
  Serial.println("RTC Initialized.\n");
#endif

#if SDCARD
  digitalWrite(BUILTIN_LED,HIGH);
  bool memoryInitialized = memory.initialize();
  success = success && memoryInitialized;
  delay(100); digitalWrite(BUILTIN_LED,LOW); delay(100);
  if (memoryInitialized)
  {
    pumpIdInit();
    memory.getFreeSpace();
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
  counterArray = Counter::createCounters(COUNTERS);
#endif

  Serial.println("Setup Done!\n");

#if PARTICLE_UNIT_TESTS
  tests();
#endif

  if(success)
    digitalWrite(BUILTIN_LED,LOW);
  else
    digitalWrite(BUILTIN_LED,HIGH);
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

#if TEMPERATURE
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
#ifdef PARTICLE_H
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
#else  //electron
  //Responding to a request from LoRa
  packets[0] = counter1.pack();
  packets[1] = counter2.pack();
  packets[2] = counter3.pack();
  packets[3] = counter4.pack();
  packets[4] = counter5.pack();
  packets[5] = counter6.pack();
  LoRaStatus = responder.respond(packets, NUMBER_OF_PACKETS);
#endif // electron
#else  //LORA
  delay(1000);
#endif //LORA

#ifdef PARTICLE_H
#if EN_GSM
  gsm.Publish(String(pumpId), message);
#endif
#endif

  //restarter.takeAction(LoRaStatus);
  Serial.println();
}
#endif // UNIT_TEST
