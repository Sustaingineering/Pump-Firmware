/*
 * Content: class that controls SD Card module
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

/*
 * SD Card -> ESP32
 * 5V      -> 3V3
 * GND     -> GND
 * MOSI    -> D23
 * SS      -> D5
 * SCK     -> D18
 * MISO    -> D19
 */

#pragma once
#include <Arduino.h>

class SdCard
{
private:
    class Impl;
    Impl *m_pImpl;
    int SD_CS_PIN;

public:
    SdCard(const int SdCardSelectPin);
    bool initialize();
    uint64_t getFreeSpace();
    void listDir(const char *dirname, uint8_t levels);
    bool createDir(const char *path);
    bool removeDir(const char *path);
    char *readFile(const char *path);
    bool writeFile(const char *path, const char *message);
    bool appendFile(const char *path, const char *message);
    bool renameFile(const char *path1, const char *path2);
    bool deleteFile(const char *path);
    bool testFileIO(const char *path);
    bool handleOverflow();
};
