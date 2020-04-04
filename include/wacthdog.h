/*
 * Content: class that restart ESP32 automatically based on threshold of an event occurance.
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>

class watchdog
{
private:
    int m_counter;
    int m_threshold;
public:
    watchdog(int);
    void takeAction(bool);
};

watchdog::watchdog(int threshold): m_counter(0), m_threshold(threshold)
{}

void watchdog::takeAction(bool status)
{
    if (!status)
    {
        m_counter = m_counter + 1;
    }
    
    if (m_counter >= m_threshold)
    {
        Serial.println("Resetting ESP32...");
        ESP.restart();
    }
}