#include "tests.h"
#include "GSMTests.h"
#include "TempTests.h"

void tests() {
    Serial.println("Hello Unit Tests");
    // Test that extern objects are available
    Serial.printlnf("%s", rtc.getTimeStamp().c_str());
#if EN_GSM
    testGsm();
#endif
#if TEMPERATURE
    testTemp();
#endif
    Serial.println("----- TESTING COMPLETE -----");
}

void run_test(bool (*test)(), char *test_name)
{
    Serial.printf("UNIT TEST: %s\n", test_name);
    if (test())
        Serial.printf("%s [PASSED]\n", test_name);
}

void printTestStart(String tag) {
    Serial.printlnf("----- START OF %s TEST -----", tag.c_str());
}

void printTestEnd(String tag) {
    Serial.printlnf("------ END OF %s TEST ------", tag.c_str());
}

void printTestFailed(String tag) {
    Serial.println("TEST FAILED");
    printTestEnd(tag);
}

void printTestPassed(String tag) {
    Serial.println("TEST PASSED");
    printTestEnd(tag);
}