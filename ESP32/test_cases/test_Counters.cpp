#include "Counter.h"
#include "PinConfig.h"
#include <unity.h>

Counter **counters = NULL;

void cleanUp_Counters()
{
    if (counters == NULL)
        return;

    for (int i = 0; i < 3; i++)
    {
        delete counters[i];
    }
    
    delete counters;

    counters = NULL;
}

void initCounters()
{
    counters = Counter::createCounters(3);
    TEST_ASSERT_NOT_NULL_MESSAGE(counters, "Failed to create Counters");
}

void test_Counter_read()
{
    if (counters == NULL)
        TEST_FAIL_MESSAGE("Counters failed to initialize");

    String counter0 = "0.10,";
    String counter1 = "1.10,";
    String counter2 = "2.10,";

    if (counter0 != counters[0]->read())
        TEST_FAIL_MESSAGE("Counter read value not as expected");
    if (counter1 != counters[1]->read())
        TEST_FAIL_MESSAGE("Counter read value not as expected");
    if (counter2 != counters[2]->read())
        TEST_FAIL_MESSAGE("Counter read value not as expected");
}