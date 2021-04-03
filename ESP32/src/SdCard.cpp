#include "SdCard.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "vfs_api.h"
#include "PinConfig.h"


class SdCard::Impl
{
private:
    SDFS fs;
    SPIClass m_spi;
    int m_SdCsPin;

public:
    Impl(int);
    bool initialize();
    void listDir(const char *dirname, uint8_t levels);
    bool createDir(const char *path);
    bool removeDir(const char *path);
    char *readFile(const char *path);
    bool writeFile(const char *path, const char *message);
    bool appendFile(const char *path, const char *message);
    bool renameFile(const char *path1, const char *path2);
    bool deleteFile(const char *path);
    bool testFileIO(const char *path);
    uint64_t getFreeSpace();
    bool handleOverflow();
};

SdCard::Impl::Impl(int SdCsPin) : fs(FSImplPtr(new VFSImpl())), m_spi(SDCARD_SPI_INTERFACE), m_SdCsPin(SdCsPin)
{
}

bool SdCard::Impl::initialize()
{
    Serial.println("Initializing SD Card...");

    int NUM_ATTEMPTS = 5;

    for (int i = 0; i < NUM_ATTEMPTS; i++)
    {
        if (!fs.begin(m_SdCsPin, m_spi, 4000000U, "/sd", 5))
        {
            Serial.printf("Card Mount Failed. Trying again in 1 sec. %d\n", i);
            delay(1000);
            
            if (i == NUM_ATTEMPTS - 1)
                return false;

        } else
            break;
    }

    uint8_t cardType = fs.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return false;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = fs.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.printf("Total space: %lluMB\n", fs.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", fs.usedBytes() / (1024 * 1024));
    listDir("/", 0);
    
    Serial.println("Initialized SD Card.");

    return true;
}

void SdCard::Impl::listDir(const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

bool SdCard::Impl::createDir(const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
        return true;
    }
    else
    {
        Serial.println("mkdir failed");
        return false;
    }
}

bool SdCard::Impl::removeDir(const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
        return true;
    }
    else
    {
        Serial.println("rmdir failed");
        return false;
    }
}

char *SdCard::Impl::readFile(const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return NULL;
    }

    char *buf = (char *)malloc(sizeof(char) * (file.size() + 1));

    Serial.print("Read from file: ");

    int i;

    for (i = 0; file.available(); i++)
        buf[i] = file.read();

    buf[i] = '\0';

    Serial.printf("%s\n", buf);

    file.close();

    return buf;
}

bool SdCard::Impl::writeFile(const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return false;
    }

    int bytesWritten = file.print(message);

    file.close();

    if (bytesWritten)
    {
        Serial.println("File written");
        return true;
    }
    else
    {
        Serial.println("Write failed");
        return false;
    }
}

bool SdCard::Impl::appendFile(const char *path, const char *message)
{
    handleOverflow();

    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return false;
    }

    int bytesWritten = file.print(message);

    file.close();

    if (bytesWritten)
    {
        Serial.println("Message appended");
        return true;
    }
    else
    {
        Serial.println("Append failed");
        return false;
    }
}

bool SdCard::Impl::renameFile(const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("File renamed");
        return true;
    }
    else
    {
        Serial.println("Rename failed");
        return false;
    }
}

bool SdCard::Impl::deleteFile(const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path))
    {
        Serial.println("File deleted");
        return true;
    }
    else
    {
        Serial.println("Delete failed");
        return false;
    }
}

bool SdCard::Impl::testFileIO(const char *path)
{
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;

    if (file)
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    }
    else
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return false;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++)
    {
        file.write(buf, 512);
    }
    end = millis() - start;

    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);

    file.close();

    return true;
}

uint64_t SdCard::Impl::getFreeSpace()
{
    uint64_t freeBytes = fs.totalBytes() - fs.usedBytes();

    //for testing
    Serial.printf("Remaining space: %llu Total Space: %llu Used Space: %d \n",
                            freeBytes, fs.totalBytes(), fs.usedBytes());
    
    return freeBytes;
}

bool SdCard::Impl::handleOverflow()
{
    int BUFFER_BYTES = 1024 * 1024 * 15; // 15 Megabytes ~ 3 days worth of logs
    
    #if UNIT_TEST
    if (true)
    #else
    if (getFreeSpace() < BUFFER_BYTES)
    #endif
    {
        File root = fs.open("/");

        String pumpIdFile("/pump-id.txt");

        int TO_DELETE = 3;

        for (int i = 0; i < TO_DELETE; i++)
        {
            File entry = root.openNextFile();

            String fileName(entry.name());

            while (entry.isDirectory() || pumpIdFile == fileName) 
            {
                Serial.println("Skipping " + fileName);

                entry.close();

                entry = root.openNextFile();

                fileName = entry.name();
            }

            deleteFile(entry.name());
            
            entry.close();
        }

        root.close();

        return true;
    }

    return false;
}

SdCard::SdCard(const int SdCardSelectPin):
m_pImpl(new SdCard::Impl(SdCardSelectPin))
{}

bool SdCard::initialize()
{
    return m_pImpl->initialize();
}

void SdCard::listDir(const char *dirname, uint8_t levels)
{
    m_pImpl->listDir(dirname, levels);
}

bool SdCard::createDir(const char *path)
{
    return m_pImpl->createDir(path);
}

bool SdCard::removeDir(const char *path)
{
    return m_pImpl->removeDir(path);
}

char *SdCard::readFile(const char *path)
{
    return m_pImpl->readFile(path);
}

bool SdCard::writeFile(const char *path, const char *message)
{
    return m_pImpl->writeFile(path, message);
}

bool SdCard::appendFile(const char *path, const char *message)
{
    return m_pImpl->appendFile(path, message);
}

bool SdCard::renameFile(const char *path1, const char *path2)
{
    return m_pImpl->renameFile(path1, path2);
}

bool SdCard::deleteFile(const char *path)
{
    return m_pImpl->deleteFile(path);
}

bool SdCard::testFileIO(const char *path)
{
    return m_pImpl->testFileIO(path);
}

uint64_t SdCard::getFreeSpace()
{
    return m_pImpl->getFreeSpace();
}

bool SdCard::handleOverflow()
{
    return m_pImpl->handleOverflow();
}
