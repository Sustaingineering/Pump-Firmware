/*
 * Content: Partile Gsm wrapper
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>

#define MAX_TOTAL_OPERATIONS 100000
#define TOTAL_PARTICLES 1
#define NUM_DAYS_IN_MONTH 31
#define MAX_OPERATIONS_PER_DAY ((MAX_TOTAL_OPERATIONS / NUM_DAYS_IN_MONTH) / TOTAL_PARTICLES)
#define MAX_MESSAGE_SIZE 60 // bytes, safe overestimate, typically 48-50 bytes at most
#define MAX_BYTES_PER_DATA_OPERATION 622
#define MAX_HEADER_SIZE 40
#define MAX_DATA_BYTES_SENT (MAX_BYTES_PER_DATA_OPERATION - MAX_HEADER_SIZE)
#define TOTAL_SECONDS_DAY 24 * 60 * 60
#define TOTAL_MESSAGES_CAP (MAX_DATA_BYTES_SENT / MAX_MESSAGE_SIZE)
#define CEILING(x,y) (((x) + (y) - 1) / (y))
#define TIME_BTWN_MESSAGES CEILING((TOTAL_SECONDS_DAY / MAX_OPERATIONS_PER_DAY), TOTAL_MESSAGES_CAP)

class Gsm
{
private:
    bool m_isConnected;
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
    Gsm(bool isConnected);

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