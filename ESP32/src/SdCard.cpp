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
    void initialize();
    void listDir(const char * dirname, uint8_t levels);
    void createDir(const char * path);
    void removeDir(const char * path);
    char *readFile(const char * path);
    void writeFile(const char * path, const char * message);
    void appendFile(const char * path, const char * message);
    void renameFile(const char * path1, const char * path2);
    void deleteFile(const char * path);
    void testFileIO(const char * path);
    int remainingSpace();
};

SdCard::Impl::Impl(int SdCsPin) : fs(FSImplPtr(new VFSImpl())), m_spi(SDCARD_SPI_INTERFACE), m_SdCsPin(SdCsPin)
{
}

void SdCard::Impl::initialize()
{
    while (!fs.begin(m_SdCsPin, m_spi, 4000000U, "/sd", 5))
    {
        Serial.println("Card Mount Failed. Trying again in 1 sec.");
        delay(1000);
    }
    uint8_t cardType = fs.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
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

void SdCard::Impl::createDir(const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void SdCard::Impl::removeDir(const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
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

    char *buf = (char *) malloc(sizeof(char) * (file.size() + 1));

    Serial.print("Read from file: ");
    
    int i;
    
    for (i = 0; file.available(); i++)
        buf[i] = file.read();

    buf[i] = '\0';

    Serial.printf("%s\n", buf);

    file.close();

    return buf;
}

void SdCard::Impl::writeFile(const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

void SdCard::Impl::appendFile(const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

void SdCard::Impl::renameFile(const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("File renamed");
    }
    else
    {
        Serial.println("Rename failed");
    }
}

void SdCard::Impl::deleteFile(const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}

void SdCard::Impl::testFileIO(const char *path)
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
    }

    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
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
}

int SdCard::Impl::remainingSpace()
{
    int freeBytes = fs.totalBytes()-fs.usedBytes();
    int freeMegaBytes = freeBytes/(1024*1024);    //1024 bytes in Gigabyte 1024 Gigabytes in Megabyte

    //for testing
    Serial.println(freeMegaBytes);
    
    return freeMegaBytes;
}

SdCard::SdCard(const int SdCardSelectPin):
m_pImpl(new SdCard::Impl(SdCardSelectPin))
{}

void SdCard::initialize()
{
    m_pImpl->initialize();
}

void SdCard::listDir(const char *dirname, uint8_t levels)
{
    m_pImpl->listDir(dirname, levels);
}
void SdCard::createDir(const char *path)
{
    m_pImpl->createDir(path);
}

void SdCard::removeDir(const char *path)
{
    m_pImpl->removeDir(path);
}

char * SdCard::readFile(const char *path)
{
    return m_pImpl->readFile(path);
}

void SdCard::writeFile(const char *path, const char *message)
{
    m_pImpl->writeFile(path, message);
}

void SdCard::appendFile(const char *path, const char *message)
{
    m_pImpl->appendFile(path, message);
}

void SdCard::renameFile(const char *path1, const char *path2)
{
    m_pImpl->renameFile(path1, path2);
}

void SdCard::deleteFile(const char *path)
{
    m_pImpl->deleteFile(path);
}

void SdCard::testFileIO(const char *path)
{
    m_pImpl->testFileIO(path);
}

int SdCard::remainingSpace()
{
    return m_pImpl->remainingSpace();
}