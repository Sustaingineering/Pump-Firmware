#include "Temperature.h"
#include "PinConfig.h"
#include <unity.h>

Temperature *temp = NULL;

void initTemperature()
{
    temp = new Temperature(TEMP_PIN, digital, "Temperature", "Celsius", 't');
}

void cleanUp_Temperature()
{
    if (temp == NULL)
        return;
    delete temp;
    temp = NULL;
}

void test_Temp_read()
{
    if (temp == NULL)
        TEST_FAIL_MESSAGE("Temp Sensor did not initialize properly");

    String read = temp->read();
    String err = "nan,";
    String print = "Temperature: " + read;
    Serial.println(print);
    if (read == err)
        TEST_FAIL_MESSAGE("Cannot read from Temperature Sensor");
}

void test_Temp_failRead()
{
    if (temp == NULL)
        TEST_FAIL_MESSAGE("Temp Sensor did not initialize properly");

    String read = temp->read();
    String err = "nan,";
    if (read != "nan,")
        TEST_FAIL_MESSAGE("Should return nan");
}
