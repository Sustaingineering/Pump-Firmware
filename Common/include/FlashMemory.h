/*
 * Content: Flash Memory API
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once

#include <Arduino.h>
#include <fcntl.h>
#include <sys/stat.h>

class FlashMemory
{
private:
    static void PrintError(const char* functionName);
    static int OpenFile(const char* filePath);
    static void CloseFile(int fd);
public:
    static off_t FileSize(const char* filePath);
    static void WriteFile(const char* filePath, const void* buffer, off_t offset, size_t bytes);
    static void ReadFile(const char* filePath, void* buffer, off_t offset, size_t bytes);
    static void PrintFile(const char* filePath, size_t chunkSize, void (*print)(void*));
    static void RemoveFile(const char* filePath);
};