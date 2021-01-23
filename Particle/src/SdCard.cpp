#include "SdCard.h"
#include <SPI.h>
#include "SdFat.h"


class SdCard::Impl
{
private:
    SdFat m_Sd;
    int m_SdCsPin;
public:
    Impl(int);
    void initialize();
    void listDir(const char * dirname, uint8_t levels);
    void createDir(const char * path);
    void removeDir(const char * path);
    char * readFile(const char * path);
    void writeFile(const char * path, const char * message);
    void appendFile(const char * path, const char * message);
    void renameFile(const char * path1, const char * path2);
    void deleteFile(const char * path);
    void testFileIO(const char * path);
};

SdCard::Impl::Impl(int SdCsPin):
 m_Sd(), m_SdCsPin(SdCsPin)
{}

void SdCard::Impl::initialize()
{ 
    Serial.print("Initializing SD card...");

    if (!m_Sd.begin(m_SdCsPin)) {
    Serial.println("initialization failed!");
    return;
    }
    Serial.println("initialization done.");
    listDir(NULL, 0);
}

void SdCard::Impl::listDir(const char * /*dirname*/, uint8_t /*levels*/)
{
    m_Sd.ls(LS_SIZE);
}

void SdCard::Impl::createDir(const char * path)
{
    Serial.println(("Error: " + String(__func__) + " is not implemented.").c_str());
}

void SdCard::Impl::removeDir(const char * path)
{
    Serial.println(("Error: " + String(__func__) + " is not implemented.").c_str());
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

void SdCard::Impl::writeFile(const char * path, const char * message)
{
    Serial.println(("Error: " + String(__func__) + " is not implemented.").c_str());
}

void SdCard::Impl::appendFile(const char * path, const char * message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = m_Sd.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void SdCard::Impl::renameFile(const char * path1, const char * path2)
{
    Serial.println(("Error: " + String(__func__) + " is not implemented.").c_str());
}

void SdCard::Impl::deleteFile(const char * path)
{
    Serial.println(("Error: " + String(__func__) + " is not implemented.").c_str());
}

void SdCard::Impl::testFileIO(const char * path)
{
    Serial.println(("Error: " + String(__func__) + " is not implemented.").c_str());
}

SdCard::SdCard(const int SdCardSelectPin):
m_pImpl(new SdCard::Impl(SdCardSelectPin))
{}

void SdCard::initialize()
{
    m_pImpl->initialize();
}

void SdCard::listDir(const char * dirname, uint8_t levels)
{
    m_pImpl->listDir(dirname, levels);
}
void SdCard::createDir(const char * path)
{
    m_pImpl->createDir(path);
}

void SdCard::removeDir(const char * path)
{
    m_pImpl->removeDir(path);
}

char * SdCard::readFile(const char * path)
{
    return m_pImpl->readFile(path);
}

void SdCard::writeFile(const char * path, const char * message)
{
    m_pImpl->writeFile(path, message);
}

void SdCard::appendFile(const char * path, const char * message)
{
    m_pImpl->appendFile(path, message);
}

void SdCard::renameFile(const char * path1, const char * path2)
{
    m_pImpl->renameFile(path1, path2);
}

void SdCard::deleteFile(const char * path)
{
    m_pImpl->deleteFile(path);
}

void SdCard::testFileIO(const char * path)
{
    m_pImpl->testFileIO(path);
}
