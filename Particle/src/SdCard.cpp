#include "SdCard.h"
#include <SPI.h>
#include "SdFat.h"
#include <libgen.h>

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
    Serial.print("Initializing SD card...");

    if (!m_Sd.begin(m_SdCsPin)) {
        Serial.println("initialization failed!");
        return false;
    }
    Serial.println("initialization successful!.");
    listDir(NULL, 0);
    uint64_t remaining_mem = getFreeSpace();
    if (remaining_mem > 0) { // negative means failure
      Serial.printlnf("Free space remaining on SD Card: %f MB", remaining_mem / (1024.0 * 1024.0));
    } else {
      Serial.println("Unable to get remaining size");
    }
    return true;
}

uint64_t SdCard::Impl::getFreeSpace()
{
    uint64_t clusterSize = BLOCK_SIZE * m_Sd.vol()->blocksPerCluster();
    Serial.print("Total space bytes: ");
    Serial.println(clusterSize * m_Sd.vol()->clusterCount());
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
            Serial.println("Failed to open file, but it should exist");
            return false;
        }
        
        if(file.isDir())
        {
            Serial.println("Directory already exists, cannot create new directory");
        } 
        else if (file.isFile())
        {
            Serial.println("File with the same name already exists, cannot create new directory");
        }

        file.close();

        return false;
    }

    bool res = m_Sd.mkdir(path);
    res ? Serial.println("Directory created") : Serial.println("Directory creation failed");

    return res;
}

bool SdCard::Impl::removeDir(const char * path)
{
    bool res = m_Sd.rmdir(path);
    res ? Serial.println("Directory removed") : Serial.println("Directory remove failed");

    return res;
}

char * SdCard::Impl::readFile(const char * path)
{
    Serial.printf("Opening file to read: %s\n", path);

    File file = m_Sd.open(path, FILE_READ);
    if(!file){
        Serial.println("Failed to open file for reading");
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
    Serial.printf("Writing to: %s\n", path);

    // Open (new) file with length truncated to 0
    File file = m_Sd.open(path, O_CREAT | O_RDWR | O_TRUNC);
    if(!file){
        Serial.println("Failed to open file for writing");
        return false;
    }

    bool res = file.print(message);
    res ? Serial.println("Message written") : Serial.println("Write failed");

    file.close();

    return res;
}

bool SdCard::Impl::appendFile(const char * path, const char * message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = m_Sd.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for appending");
        return false;
    }

    bool res = file.print(message);
    res ? Serial.println("Message appended") : Serial.println("Append failed");

    file.close();

    return res;
}

bool SdCard::Impl::renameFile(const char * path1, const char * path2)
{
    bool res = m_Sd.rename(path1, path2);
    res ? Serial.println("File renamed") : Serial.println("File rename failed");

    return res;
}

bool SdCard::Impl::deleteFile(const char * path)
{
    bool res = m_Sd.remove(path);
    res ? Serial.println("File removed") : Serial.println("File remove failed");

    return res;
}

bool SdCard::Impl::testFileIO(const char * path)
{
    char * test_msg = "This is a test message";
    char * test_file = "/testfile.txt";
    
    // Initialize tests (remove required files)
    bool test_file_exists = m_Sd.exists(test_file);
    if (test_file_exists)
    {
        bool deleted = deleteFile(test_file);
        test_file_exists = m_Sd.exists(test_file);
        if (deleted && !test_file_exists)
        {
            Serial.println("File cleaned up: Tests Initialized");
        }
        else
        {
            Serial.println("Error cleaning up files");
            return false;
        }
    } 
    else
    {
        Serial.println("File DNE: Tests Initialized");
    }

    // WRITE TEST
    bool created = writeFile(test_file, test_msg);
    test_file_exists = m_Sd.exists(test_file);
    if (created && test_file_exists)
    {
        Serial.println("Write test successful");
    }
    else
    {
        Serial.println("Failed to create and write to file");
        return false;
    }

    // READ TEST
    char * read_contents = readFile(test_file);
    if (read_contents != NULL)
    {
        bool content_same = strcmp(read_contents, test_msg);
        if (!content_same)
        {
            Serial.printlnf("Read failed, file contents different: %s", read_contents);
            return false;
        }
        else
        {
            Serial.println("Read test successful");
        }
    }
    else
    {
        Serial.printlnf("Read failed, returned NULL pointer");
        return false;
    }



    // Serial.println(("Error: " + String(__func__) + " is not implemented.").c_str());
    return true;
}

SdCard::SdCard(const int SdCardSelectPin):
m_pImpl(new SdCard::Impl(SdCardSelectPin))
{}

bool SdCard::initialize()
{
    return m_pImpl->initialize();
}

uint64_t SdCard::getFreeSpace()
{
    return m_pImpl->getFreeSpace();
}

void SdCard::listDir(const char * dirname, uint8_t levels)
{
    m_pImpl->listDir(dirname, levels);
}

bool SdCard::createDir(const char * path)
{
    return m_pImpl->createDir(path);
}

bool SdCard::removeDir(const char * path)
{
    return m_pImpl->removeDir(path);
}

char * SdCard::readFile(const char * path)
{
    return m_pImpl->readFile(path);
}

bool SdCard::writeFile(const char * path, const char * message)
{
    return m_pImpl->writeFile(path, message);
}

bool SdCard::appendFile(const char * path, const char * message)
{
    return m_pImpl->appendFile(path, message);
}

bool SdCard::renameFile(const char * path1, const char * path2)
{
    return m_pImpl->renameFile(path1, path2);
}

bool SdCard::deleteFile(const char * path)
{
    return m_pImpl->deleteFile(path);
}

bool SdCard::testFileIO(const char * path)
{
    return m_pImpl->testFileIO(path);
}
