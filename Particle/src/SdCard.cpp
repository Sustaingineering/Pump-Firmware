#include "SdCard.h"
#include <SPI.h>
#include "SdFat.h"
#include <libgen.h>
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_SDCARD_SWITCH
#define BLOCK_SIZE 512

class SdCard::Impl
{
private:
    SdFat m_Sd;
    int m_SdCsPin;
public:
    Impl(int);
    bool initialize();
    uint64_t getFreeSpace();
    void listDir(const char * dirname, uint8_t levels);
    bool createDir(const char * path);
    bool removeDir(const char * path);
    char * readFile(const char * path);
    bool writeFile(const char * path, const char * message);
    bool appendFile(const char * path, const char * message);
    bool renameFile(const char * path1, const char * path2);
    bool deleteFile(const char * path);
    bool testFileIO(const char * path);
};

SdCard::Impl::Impl(int SdCsPin):
 m_Sd(), m_SdCsPin(SdCsPin)
{}

bool SdCard::Impl::initialize()
{
    if (!m_Sd.begin(m_SdCsPin)) {
        LOGGER("SD card initialization failed!");
        return false;
    }
    LOGGER("Initialized SD card");
    listDir(NULL, 0);
    uint64_t remaining_mem = getFreeSpace();
    if (remaining_mem > 0) { // negative means failure
      LOGGER("Free space remaining on SD Card: " + String(remaining_mem / (1024.0 * 1024.0)) +" MB");
    } else {
      LOGGER("Unable to get remaining size");
    }
    return true;
}

uint64_t SdCard::Impl::getFreeSpace()
{
    uint64_t clusterSize = BLOCK_SIZE * m_Sd.vol()->blocksPerCluster();
    LOGGER("Total space bytes: " + String((unsigned long)(clusterSize * m_Sd.vol()->clusterCount())));
    return clusterSize * m_Sd.vol()->freeClusterCount();
}

void SdCard::Impl::listDir(const char * /*dirname*/, uint8_t /*levels*/)
{
    m_Sd.ls(LS_SIZE);
}

bool SdCard::Impl::createDir(const char * path)
{
    bool exists = m_Sd.exists(path);
    if (exists)
    {
        File file = m_Sd.open(path, FILE_READ);
        if(!file){
            LOGGER("Failed to open file, but it should exist");
            return false;
        }
        
        if(file.isDir())
        {
            LOGGER("Directory already exists, cannot create new directory");
        } 
        else if (file.isFile())
        {
            LOGGER("File with the same name already exists, cannot create new directory");
        }

        file.close();

        return false;
    }

    bool res = m_Sd.mkdir(path);
    res ? LOGGER("Directory created") : LOGGER("Directory creation failed");

    return res;
}

bool SdCard::Impl::removeDir(const char * path)
{
    bool res = m_Sd.rmdir(path);
    res ? LOGGER("Directory removed") : LOGGER("Directory remove failed");

    return res;
}

char * SdCard::Impl::readFile(const char * path)
{
    LOGGER("Opening file to read: " + String(path));

    File file = m_Sd.open(path, FILE_READ);
    if(!file){
        LOGGER("Failed to open file for reading");
        return NULL;
    }

    int file_size = file.size();
    char * file_contents = (char*) malloc(file_size * sizeof(int) + 1);
    int byte_read = file.read();
    int read_idx = 0;

    // read() returns -1 at EOF
    while(byte_read != -1) {
        file_contents[read_idx] = (char) byte_read;
        read_idx++;

        // Read the next byte
        byte_read = file.read();
    }

    file_contents[file_size] = '\0';
    
    file.close();

    return file_contents;
}

bool SdCard::Impl::writeFile(const char * path, const char * message)
{
    LOGGER("Writing to: " + String(path));

    // Open (new) file with length truncated to 0
    File file = m_Sd.open(path, O_CREAT | O_RDWR | O_TRUNC);
    if(!file){
        LOGGER("Failed to open file for writing");
        return false;
    }

    bool res = file.print(message);
    res ? LOGGER("Message written") : LOGGER("Write failed");

    file.close();

    return res;
}

bool SdCard::Impl::appendFile(const char * path, const char * message)
{
    LOGGER("Appending to file: " + String(path));

    File file = m_Sd.open(path, FILE_WRITE);
    if(!file){
        LOGGER("Failed to open file for appending");
        return false;
    }

    bool res = file.print(message);
    res ? LOGGER("Message appended") : LOGGER("Append failed");

    file.close();

    return res;
}

bool SdCard::Impl::renameFile(const char * path1, const char * path2)
{
    bool res = m_Sd.rename(path1, path2);
    res ? LOGGER("File renamed") : LOGGER("File rename failed");

    return res;
}

bool SdCard::Impl::deleteFile(const char * path)
{
    bool res = m_Sd.remove(path);
    res ? LOGGER("File removed") : LOGGER("File remove failed");

    return res;
}

bool SdCard::Impl::testFileIO(const char * path)
{
    String test_msg = "This is a test message";
    String test_file = "/testfile.txt";
    
    // Initialize tests (remove required files)
    bool test_file_exists = m_Sd.exists(test_file.c_str());
    if (test_file_exists)
    {
        bool deleted = deleteFile(test_file.c_str());
        test_file_exists = m_Sd.exists(test_file.c_str());
        if (deleted && !test_file_exists)
        {
            LOGGER("File cleaned up: Tests Initialized");
        }
        else
        {
            LOGGER("Error cleaning up files");
            return false;
        }
    } 
    else
    {
        LOGGER("File DNE: Tests Initialized");
    }

    // WRITE TEST
    bool created = writeFile(test_file.c_str(), test_msg.c_str());
    test_file_exists = m_Sd.exists(test_file.c_str());
    if (created && test_file_exists)
    {
        LOGGER("Write test successful");
    }
    else
    {
        LOGGER("Failed to create and write to file");
        return false;
    }

    // READ TEST
    char * read_contents = readFile(test_file.c_str());
    if (read_contents != NULL)
    {
        bool content_same = strcmp(read_contents, test_msg.c_str());
        if (!content_same)
        {
            LOGGER("Read failed, file contents different: " + String(read_contents));
            return false;
        }
        else
        {
            LOGGER("Read test successful");
        }
    }
    else
    {
        LOGGER("Read failed, returned NULL pointer");
        return false;
    }

    return true;
}

SdCard::SdCard(bool isConnected, const int SdCardSelectPin):
    m_isConnected(isConnected), m_pImpl(new SdCard::Impl(SdCardSelectPin))
{}

bool SdCard::initialize()
{
    if (!m_isConnected)
    {
        return false;
    }
    return m_pImpl->initialize();
}

uint64_t SdCard::getFreeSpace()
{
    if (!m_isConnected)
    {
        return 0;
    }
    return m_pImpl->getFreeSpace();
}

void SdCard::listDir(const char * dirname, uint8_t levels)
{
    if (!m_isConnected)
    {
        return;
    }
    m_pImpl->listDir(dirname, levels);
}

bool SdCard::createDir(const char * path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->createDir(path);
}

bool SdCard::removeDir(const char * path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->removeDir(path);
}

char * SdCard::readFile(const char * path)
{
    if (!m_isConnected)
    {
        return nullptr;
    }
    return m_pImpl->readFile(path);
}

bool SdCard::writeFile(const char * path, const char * message)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->writeFile(path, message);
}

bool SdCard::appendFile(const char * path, const char * message)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->appendFile(path, message);
}

bool SdCard::renameFile(const char * path1, const char * path2)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->renameFile(path1, path2);
}

bool SdCard::deleteFile(const char * path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->deleteFile(path);
}

bool SdCard::testFileIO(const char * path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->testFileIO(path);
}
