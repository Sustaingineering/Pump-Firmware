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
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "vfs_api.h"
#include "PinConfig.h"

class SdCard {
private:
    SDFS m_SD;
    SPIClass m_spi;
    void listDir_   (fs::FS &fs, const char * dirname, uint8_t levels);
    void createDir_ (fs::FS &fs, const char * path);
    void removeDir_ (fs::FS &fs, const char * path);
    void readFile_  (fs::FS &fs, const char * path);
    void writeFile_ (fs::FS &fs, const char * path, const char * message);
    void appendFile_(fs::FS &fs, const char * path, const char * message);
    void renameFile_(fs::FS &fs, const char * path1, const char * path2);
    void deleteFile_(fs::FS &fs, const char * path);
    void testFileIO_(fs::FS &fs, const char * path);
public:
    SdCard();
    void initialize();
    void listDir   (const char * dirname, uint8_t levels);
    void createDir (const char * path);
    void removeDir (const char * path);
    void readFile  (const char * path);
    void writeFile (String, String);
    void appendFile(String, String);
    void renameFile(const char * path1, const char * path2);
    void deleteFile(const char * path);
    void testFileIO(const char * path);
};
