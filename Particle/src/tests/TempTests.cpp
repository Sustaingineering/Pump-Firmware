#include "TempTests.h"

static Temperature *temp = NULL;

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

void testTemp()
{
    initTemperature();
    test_Temp_read();
    cleanUp_Temperature();
}
