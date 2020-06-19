#include <Arduino.h>
#include "config.h"
#include "Restarter.h"
#include "watch.h"
#include "SdCard.h"
#include "farmSensor.h"
#include "temp.h"
#include "volt.h"
#include "current.h"
#include "LoRaTransceiver.h"

//Global Objects
//Restarter restarter(5);
String message;

#if CURRENT
current hall_effect(12);
#endif

#if VOLTAGE
volt volt_divider(12, 1000, 1000);
#endif

#if TEMP
temp thermocouple(4); //pretty slow response and depends greatly on the surface temperature of the thermocouple tip
#endif

#if RTC
watch rtc(false);
#endif

#if SDCARD
SdCard memory;
#endif

#if LORA
#define PUMP_ID 0
#define LoRa_SECRET_WORD 0xF3
bool LoRaStatus;
packet packets[NUMBER_OF_PACKETS];
#if GSM
LoRaTransceiver requester(15, 27, 26, LoRa_SECRET_WORD);
#else
LoRaTransceiver responder(15, 27, 26, LoRa_SECRET_WORD, PUMP_ID);
#endif //GSM
#endif //LORA

#if COUNTERS
farmSensor counter1(0, counter, "Counter1", "T", 'c');
farmSensor counter2(0, counter, "Counter2", "T", 'd');
farmSensor counter3(0, counter, "Counter3", "T", 'e');
farmSensor counter4(0, counter, "Counter4", "T", 'f');
farmSensor counter5(0, counter, "Counter4", "T", 'g');
farmSensor counter6(0, counter, "Counter4", "T", 'h');
#endif
//Sensors Constructors go here.

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nHello Sustaingineering!\n");

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
  
#if RTC
  Serial.println("Initializing RTC...");
  rtc.initialize();
  Serial.println("RTC Initialized.\n");
#endif

#if SDCARD
  Serial.println("Initializing MicroSD Card...");
  memory.initialize();
  Serial.println("MicroSD Card Initialized.\n");
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

  //Sensors Initializers go here.
  
  Serial.println("Setup Done!\n");
}

void loop()
{
  message = "";
  //Sampling Sensors
#if COUNTERS
  message += counter1.read();
  message += counter2.read();
  message += counter3.read();
  message += counter4.read();
  message += counter5.read();
  message += counter6.read();
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

#if RTC
  message += rtc.getTimeStamp();
#endif

  message += String("\n");
  Serial.print(message);
  
#if SDCARD
  //Writing on Sd Card
  memory.appendFile("/" + rtc.getDate() + ".txt", message);
#endif

#if LORA
#if GSM
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
  delay(100); //JAIDEN: REPLACE THIS DELAY WITH GSM CODE THAT SENDS SMS 
#else //GSM
  //Responding to a request from LoRa
  packets[0] = counter1.pack();
  packets[1] = counter2.pack();
  packets[2] = counter3.pack();
  packets[3] = counter4.pack();
  packets[4] = counter5.pack();
  packets[5] = counter6.pack();
  LoRaStatus = responder.respond(packets, NUMBER_OF_PACKETS);
#endif //GSM
#else //LORA
  delay(1000);
#endif //LORA

  //restarter.takeAction(LoRaStatus);
  Serial.println();
}
