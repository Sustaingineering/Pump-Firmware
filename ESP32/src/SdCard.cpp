#include "SdCard.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "vfs_api.h"
#include "PinConfig.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_SDCARD_SWITCH

class SdCard::Impl
{
private:
    SDFS fs;
    SPIClass m_spi;
    int m_SdCsPin;
    bool is_Working;
public:
    Impl(int);
    bool initialize();
    void listDir(const char *dirname, uint8_t levels);
    bool createDir(const char *path);
    bool removeDir(const char *path);
    char *readFile(const char *path);
    bool writeFile(const char *path, const char *message);
    bool appendFile(const char *path, const char *message, bool isLogger);
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
    int NUM_ATTEMPTS = 5;

    for (int i = 0; i < NUM_ATTEMPTS; i++)
    {
        if (!fs.begin(m_SdCsPin, m_spi, 4000000U, "/sd", 5))
        {
            Serial.printf("Card Mount Failed. Trying again in 1 sec. %d\n", i);
            delay(1000);
            
            if (i == NUM_ATTEMPTS - 1)
            {
                is_Working = false;
                return false;
            }

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
    
    Serial.println("Initialized SD Card.");

    is_Working = true;

    return true;
}

void SdCard::Impl::listDir(const char *dirname, uint8_t levels)
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return;
    }

    LOGGER("Listing directory: " + String(dirname));

    File root = fs.open(dirname);
    if (!root)
    {
        LOGGER("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        LOGGER("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            LOGGER(file.name());
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
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return false;
    }

    LOGGER("Creating Dir: " + String(path));
    if (fs.mkdir(path))
    {
        LOGGER("Dir created");
        return true;
    }
    else
    {
        LOGGER("mkdir failed");
        return false;
    }
}

bool SdCard::Impl::removeDir(const char *path)
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return false;
    }

    LOGGER("Removing Dir: " + String(path));
    if (fs.rmdir(path))
    {
        LOGGER("Dir removed");
        return true;
    }
    else
    {
        LOGGER("rmdir failed");
        return false;
    }
}

char *SdCard::Impl::readFile(const char *path)
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return NULL;
    }

    LOGGER("Reading file: " + String(path));

    File file = fs.open(path);
    if (!file)
    {
        LOGGER("Failed to open file for reading");
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
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return false;
    }

    LOGGER("Writing file: " +  String(path));

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        LOGGER("Failed to open file for writing");
        return false;
    }

    int bytesWritten = file.print(message);

    file.close();

    if (bytesWritten)
    {
        LOGGER("File written");
        return true;
    }
    else
    {
        LOGGER("Write failed");
        return false;
    }
}

bool SdCard::Impl::appendFile(const char *path, const char *message, bool isLogger)
{
    if (!is_Working)
    {
        if (!isLogger)
            LOGGER("SD Card not initialized properly");
        return false;
    }

    #ifndef UNIT_TEST
    handleOverflow();
    #endif
    if (!isLogger)
        LOGGER("Appending to file: " +  String(path));

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        if (!isLogger)
            LOGGER("Failed to open file for appending");
        return false;
    }

    int bytesWritten = file.print(message);

    file.close();

    if (bytesWritten)
    {
        if (!isLogger)
            LOGGER("Message appended");
        return true;
    }
    else
    {
        if (!isLogger)
            LOGGER("Append failed");
        return false;
    }
}

bool SdCard::Impl::renameFile(const char *path1, const char *path2)
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return false;
    }

    LOGGER("Renaming file " + String(path1) + " to " + String(path2));
    if (fs.rename(path1, path2))
    {
        LOGGER("File renamed");
        return true;
    }
    else
    {
        LOGGER("Rename failed");
        return false;
    }
}

bool SdCard::Impl::deleteFile(const char *path)
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return false;
    }

    LOGGER("Deleting file: " + String(path));
    if (fs.remove(path))
    {
        LOGGER("File deleted");
        return true;
    }
    else
    {
        LOGGER("Delete failed");
        return false;
    }
}

bool SdCard::Impl::testFileIO(const char *path)
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return false;
    }

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
        LOGGER(String(flen) + " bytes read for " + String(end) + " ms");
        file.close();
    }
    else
    {
        LOGGER("Failed to open file for reading");
        return false;
    }

    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        LOGGER("Failed to open file for writing");
        return false;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++)
    {
        file.write(buf, 512);
    }
    end = millis() - start;

    LOGGER(String(2048 * 512) + " bytes written for " + String(end) + " ms");

    file.close();

    return true;
}

uint64_t SdCard::Impl::getFreeSpace()
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return 0;
    }

    uint64_t freeBytes = fs.totalBytes() - fs.usedBytes();

    // TODO: add a logging feature with multiple levels
    // Serial.printf("Remaining space: %llu Total Space: %llu Used Space: %d \n",
    //                         freeBytes, fs.totalBytes(), fs.usedBytes());
    
    return freeBytes;
}

bool SdCard::Impl::handleOverflow()
{
    if (!is_Working)
    {
        LOGGER("SD Card not initialized properly");
        return false;
    }
    
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
                LOGGER("Skipping " + fileName);

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

void SdCard::listDir(const char *dirname, uint8_t levels)
{
    if (!m_isConnected)
    {
        return;
    }
    m_pImpl->listDir(dirname, levels);
}

bool SdCard::createDir(const char *path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->createDir(path);
}

bool SdCard::removeDir(const char *path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->removeDir(path);
}

char *SdCard::readFile(const char *path)
{
    if (!m_isConnected)
    {
        return nullptr;
    }
    return m_pImpl->readFile(path);
}

bool SdCard::writeFile(const char *path, const char *message)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->writeFile(path, message);
}

bool SdCard::appendFile(const char *path, const char *message, bool isLogger)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->appendFile(path, message, isLogger);
}

bool SdCard::renameFile(const char *path1, const char *path2)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->renameFile(path1, path2);
}

bool SdCard::deleteFile(const char *path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->deleteFile(path);
}

bool SdCard::testFileIO(const char *path)
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->testFileIO(path);
}

uint64_t SdCard::getFreeSpace()
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->getFreeSpace();
}

bool SdCard::handleOverflow()
{
    if (!m_isConnected)
    {
        return true;
    }
    return m_pImpl->handleOverflow();
}
