#include "SDCardTests.h"

void initSDcard()
{
    if (!memory.initialize())
        TEST_FAIL_MESSAGE("SdCard failed to initialize properly.");
}

void testSdCardWriteRead()
{
    const char *testMsg = "Hello World!";
    const char *testFile = "/testSdcard.txt";

    if (!memory.writeFile(testFile, testMsg))
        TEST_FAIL_MESSAGE("Failed to write to SdCard");

    char *readMsg = memory.readFile(testFile);

    if (readMsg == NULL)
        TEST_FAIL_MESSAGE("Failed to read from SdCard");

    TEST_ASSERT_EQUAL_STRING(testMsg, readMsg);

    delete readMsg;

    memory.deleteFile(testFile);
}

void testSdCardAppendFile()
{
    const char *writeMsg = "HELLLOOO\n";
    const char *appendMsg = "GOODBYE";
    const char *fullMsg = "HELLLOOO\nGOODBYE";
    const char *testFile = "/testSdcardAppend.txt";

    if (!memory.writeFile(testFile, writeMsg))
        TEST_FAIL_MESSAGE("Failed to write to SdCard");
    

    if (!memory.appendFile(testFile, appendMsg))
        TEST_FAIL_MESSAGE("Failed to append to SdCard");
    
    char *readMsg = memory.readFile(testFile);

    if (readMsg == NULL)
        TEST_FAIL_MESSAGE("Failed to append to SdCard");

    TEST_ASSERT_EQUAL_STRING(fullMsg, readMsg);

    delete readMsg;

    memory.deleteFile(testFile);
}

void testSdCardDeleteFile()
{    
    const char *testFile = "/testSdcardDelete.txt";
    
    if (!memory.writeFile(testFile, "hi"))
        TEST_FAIL_MESSAGE("Failed to create file");
    
    if (!memory.deleteFile(testFile))
        TEST_FAIL_MESSAGE("Failed to delete file");
        
}

void testSdCardOverflow()
{
    RealTimeClock rtc;
    long time = 0; 
    rtc.initialize(time);

    for (int i = 0; i < 10; i++)
    {
        time += 86400;
        memory.writeFile(("/" + rtc.getDate() + ".txt").c_str(), "0");
        rtc.initialize(time);
    }
    
    memory.listDir("/", 0);
    memory.handleOverflow();
    memory.listDir("/", 0);

    TEST_ASSERT_NULL(memory.readFile("/1970-01-01.txt"));
    TEST_ASSERT_NULL(memory.readFile("/1970-01-02.txt"));
    TEST_ASSERT_NULL(memory.readFile("/1970-01-03.txt"));

}

