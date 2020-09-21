/*
 * Content: Partile Gsm wrapper
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>

class Gsm
{
private:
    int m_lastTimePublished = 0;
public:
    void Publish(String pumpId, String message);
};