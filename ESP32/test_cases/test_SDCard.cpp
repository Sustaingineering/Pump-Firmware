#include "SdCard.h"
#include "PinConfig.h"
#include <unity.h>
#include <RealTimeClock.h>

SdCard *memory = NULL;

void cleanUp_SDcard()
{
    if (memory == NULL)
        return;

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
    if (memory == NULL)
        TEST_FAIL_MESSAGE("SdCard did not initialize properly");

    const char *testMsg = "Hello World!";
    const char *testFile = "/test_sdcard.txt";

    if (!memory->writeFile(testFile, testMsg))
        TEST_FAIL_MESSAGE("Failed to write to SdCard");

    char *readMsg = memory->readFile(testFile);

    if (readMsg == NULL)
        TEST_FAIL_MESSAGE("Failed to read from SdCard");

    TEST_ASSERT_EQUAL_STRING(testMsg, readMsg);

    delete readMsg;

    memory->deleteFile(testFile);
}

void test_SdCard_AppendFile()
{
    if (memory == NULL)
        TEST_FAIL_MESSAGE("SdCard did not initialize properly");

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

    delete readMsg;

    memory->deleteFile(testFile);
}

void test_SdCard_deleteFile()
{
    if (memory == NULL)
        TEST_FAIL_MESSAGE("SdCard did not initialize properly");
    
    const char *testFile = "/test_sdcard_delete.txt";
    
    if (!memory->writeFile(testFile, "hi"))
        TEST_FAIL_MESSAGE("Failed to create file");
    
    if (!memory->deleteFile(testFile))
        TEST_FAIL_MESSAGE("Failed to delete file");
        
}

void test_SdCard_Overflow()
{
    RealTimeClock rtc;
    long time = 1616307852L; //2021-3-20
    rtc.initialize(time);
    for (int i = 0; i < 10; i++)
    {
        time += 86400;
        memory->writeFile(("/" + rtc.getDate() + ".txt").c_str(), "0");
        rtc.initialize(time);
    }
    memory->listDir("/", 0);
    memory->handleOverflow();
    memory->listDir("/", 0);

}