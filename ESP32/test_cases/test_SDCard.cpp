#include "SdCard.h"
#include "PinConfig.h"
#include <unity.h>

SdCard *memory = NULL;

void cleanUp_SDcard()
{
    delete memory;
    memory = NULL;
}

void initSDcard()
{
    memory = new SdCard(SDCARD_SELECT_PIN);
    TEST_ASSERT_NOT_NULL(memory);
    if (!memory->initialize())
        TEST_FAIL_MESSAGE("SdCard failed to initialize properly.");
}

void test_SdCard_WriteRead()
{
    const char *testMsg = "Hello World!";
    const char *testFile = "/test_sdcard.txt";

    if (!memory->writeFile(testFile, testMsg))
        TEST_FAIL_MESSAGE("Failed to write to SdCard");

    char *readMsg = memory->readFile(testFile);

    if (readMsg == NULL)
        TEST_FAIL_MESSAGE("Failed to read from SdCard");

    TEST_ASSERT_EQUAL_STRING(testMsg, readMsg);

    free(readMsg);

}

void test_SdCard_AppendFile()
{
    const char *writeMsg = "HELLLOOO\n";
    const char *appendMsg = "GOODBYE";
    const char *fullMsg = "HELLLOOO\nGOODBYE";
    const char *testFile = "/test_sdcard_append.txt";

    if (!memory->writeFile(testFile, writeMsg))
        TEST_FAIL_MESSAGE("Failed to write to SdCard");
    

    if (!memory->appendFile(testFile, appendMsg))
        TEST_FAIL_MESSAGE("Failed to append to SdCard");
    
    char *readMsg = memory->readFile(testFile);

    if (readMsg == NULL)
        TEST_FAIL_MESSAGE("Failed to append to SdCard");

    TEST_ASSERT_EQUAL_STRING(fullMsg, readMsg);

}

