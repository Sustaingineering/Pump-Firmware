/*
 * Content: Partile Gsm wrapper
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>

#define MAX_TOTAL_OPERATIONS 100000
#define TOTAL_PARTICLES 10
#define PARTICLE_OPERATIONS ((MAX_TOTAL_OPERATIONS / 31 ) / TOTAL_PARTICLES)
#define MESSAGE_SIZE 60
#define MAX_DATA_BYTES_SENT 540
#define TOTAL_SECONDS_DAY 86400
#define TOTAL_MESSAGES_CAP (MAX_DATA_BYTES_SENT / MESSAGE_SIZE)
#define TIME_BTWN_MESSAGES ((TOTAL_SECONDS_DAY / PARTICLE_OPERATIONS) / TOTAL_MESSAGES_CAP)

class Gsm
{
private:
    int m_counter;
    int m_timeFromLastMessage;
    String delimiter;
    String m_buffer;

    /**
     * @brief Get the total data usage (sent + received).
     * 
     * @return int Number of bytes sent and received.
     */
    int getTotalDataUsage_();

public:

    /**
     * @brief Currently only initializes private members with defined macros. 
     * Stub for future functionality. 
     */
    bool initialize();

    /**
     * @brief Calls the Particle publish method based on time and data restrictions
     * 
     * @param String pumpId: Stringified integer representing the pump ID
     * @param String message: String containing data to be published by the Particle
     * @return Published string succesfull; else empty string
     */
    String Publish(String pumpId, String message);
};