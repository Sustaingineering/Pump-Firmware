#include "tests.h"

void tests() {
    Serial.println("Hello Unit Tests");
    // Test that extern objects are available
    Serial.printlnf("%s", rtc.getTimeStamp().c_str());
    while(1);
}

void printTestStart(String tag) {
    Serial.printlnf("----- START OF %s TEST -----", tag.c_str());
}

void printTestEnd(String tag) {
    Serial.printlnf("------ END OF %s TEST ------", tag.c_str());
}