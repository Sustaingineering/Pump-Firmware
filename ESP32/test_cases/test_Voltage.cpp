#include "Voltage.h"
#include "PinConfig.h"
#include <unity.h>

Voltage *voltage = NULL;

void initVoltage()
{
    voltage = new Voltage(VOLT_PIN, analog, "Voltage", "Volts", 'V', 1000, 100000, MAX_V);
    TEST_ASSERT_NOT_NULL_MESSAGE(voltage,"Voltage Sensor failed to initialize");
}

void test_Voltage_read()
{
    if (voltage == NULL)
        TEST_FAIL_MESSAGE("Voltage Sensor was initalized incorrectly");
    
    String read = voltage->read();
    String error = "nan,";
    
    if(read == error)
        TEST_FAIL_MESSAGE("Can not read from Voltage Sensor");
}

void cleanUp_Voltage()
{
    if (voltage == NULL)
        return;
    delete voltage;
    voltage = NULL;
}