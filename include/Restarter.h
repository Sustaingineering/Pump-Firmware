/*
 * Content: class that restart ESP32 automatically based on threshold of boolean having a false value.
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

#pragma once
#include <Arduino.h>

class Restarter
{
private:
    int m_counter;
    int m_threshold;
public:
    Restarter(int);
    void takeAction(bool);
};

Restarter::Restarter(int threshold): m_counter(0), m_threshold(threshold)
{}

void Restarter::takeAction(bool status)
{
    if (!status)
    {
        m_counter = m_counter + 1;
    }
    else
    {
        m_counter = 0;
    }
    
    if (m_counter >= m_threshold)
    {
        Serial.println("Restarting ESP32...");
        ESP.restart();
    }
}