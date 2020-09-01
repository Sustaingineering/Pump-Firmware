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

class SdCard {
private:
    class Impl;
    Impl* m_pImpl;
public:
    SdCard();
    void initialize();
    void listDir(const char * dirname, uint8_t levels);
    void createDir(const char * path);
    void removeDir(const char * path);
    void readFile(const char * path);
    void writeFile(const char * path, const char * message);
    void appendFile(const char * path, const char * message);
    void renameFile(const char * path1, const char * path2);
    void deleteFile(const char * path);
    void testFileIO(const char * path);
};
