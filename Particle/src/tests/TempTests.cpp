#include "TempTests.h"

Temperature temp(TEMPERATURE_SWITCH, TEMP_PIN);


bool test_Temp_read()
{
    String read = temp.read();
    String err = "0.00,";
    String result = "Temperature: " + read;
    Serial.println(result);
    if (err == read)
        TEST_FAIL_MESSAGE("Temperature Reading Failed");

    return true;
}

void testTemp()
{
    String tag = "Temperature Sensor";
    printTestStart(tag);

    RUN_TEST(test_Temp_read);

    printTestEnd(tag);
}
