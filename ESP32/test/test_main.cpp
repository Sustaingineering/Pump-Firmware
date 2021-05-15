#include <Arduino.h>
#include "PinConfig.h"

#include <unity.h>

#include "test_cases.h"
//TEST CMD --> pio test --environment esp32doit-devkit-v1

void setup()
{
    delay(2000); // THIS IS REQUIRED FOR TESTING 

    UNITY_BEGIN();

    pinMode(BUILTIN_LED, OUTPUT);
}


void loop()
{

#if SDCARD
    RUN_TEST(initSDcard);
    RUN_TEST(test_SdCard_WriteRead);
    RUN_TEST(test_SdCard_AppendFile);
    RUN_TEST(test_SdCard_deleteFile);
    cleanUp_SDcard();
#endif

#if TEMP
    RUN_TEST(initTemperature);
    RUN_TEST(test_Temp_read);
    // Serial.println("DISCONNECT TEMPERATURE!!!");
    // delay(3000);
    // RUN_TEST(test_Temp_failRead);
    cleanUp_Temperature();
#endif

#if COUNTERS
    RUN_TEST(initCounters);
    RUN_TEST(test_Counter_read);
    cleanUp_Counters();
#endif
    
#if VOLTAGE
    RUN_TEST(initVoltage);
    RUN_TEST(test_Voltage_read);
    cleanUp_Voltage();
#endif

    UNITY_END();
}
