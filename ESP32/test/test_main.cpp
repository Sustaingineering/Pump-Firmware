#include <Arduino.h>
#include "PinConfig.h"

#include <unity.h>

#include "../test_cases/test_SDCard.cpp"
//pio test --environment esp32doit-devkit-v1

void setup()
{
    delay(2000); // THIS IS REQUIRED FOR TESTING 

    UNITY_BEGIN();

    pinMode(BUILTIN_LED, OUTPUT);
}

bool done = false;

void loop()
{
    delay(500);

#ifdef SDCARD
    RUN_TEST(initSDcard);
    RUN_TEST(test_SdCard_WriteRead);
    RUN_TEST(test_SdCard_AppendFile);
    cleanUp_SDcard();
#endif

    if (!done)
    {
        UNITY_END();
        done = true;
    }
}
