#include <Arduino.h>
#include "PinConfig.h"

#include <unity.h>

#include "../test_cases/test_SDCard.cpp"
//TEST CMD --> pio test --environment esp32doit-devkit-v1

void setup()
{
    delay(2000); // THIS IS REQUIRED FOR TESTING 

    UNITY_BEGIN();

    pinMode(BUILTIN_LED, OUTPUT);
}


void loop()
{
    delay(500);

#if SDCARD
    RUN_TEST(initSDcard);
    RUN_TEST(test_SdCard_WriteRead);
    RUN_TEST(test_SdCard_AppendFile);
    cleanUp_SDcard();
#endif
    
    UNITY_END();
}
