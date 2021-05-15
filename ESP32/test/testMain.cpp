#include <Arduino.h>
#include "PinConfig.h"

#include <unity.h>

#include "testCases.h"
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

    RUN_TEST(testSdCardWriteRead);
    RUN_TEST(testSdCardAppendFile);
    RUN_TEST(testSdCardDeleteFile);
    RUN_TEST(testSdCardOverflow);
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
    
    UNITY_END();
}
