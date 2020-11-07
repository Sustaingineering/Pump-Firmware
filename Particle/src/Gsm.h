/*
 * Content: Partile Gsm wrapper
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>

#define MAX_DATA_0 6400
#define MAX_DATA_LIMIT_MB 3
#define MAX_DATA_LIMIT_BYTES (MAX_DATA_LIMIT_MB * 1024 * 1024)
#define NUM_DAYS 31
#define NUM_HOURS 24
#define SECONDS_IN_HOUR 3600
#define DEFAULT_PERIOD 10

class Gsm
{
private:
    int m_lastTimePublished = 0;
    int m_publishPeriod = 0;
    int m_numPublish = 0;
    int m_currentUsageBytes = 0;
    int m_maxData0Bytes = 0;

    /**
     * @brief Computes the period at which the GSM module will publish messages
     * based on the maximum data limit and test-derived parameters.
     */
    void computePublishPeriod_();

    /**
     * @brief Get the total data usage (sent + received).
     * 
     * @return int Number of bytes sent and received.
     */
    int getTotalDataUsage_();

    /**
     * @brief Update private members with number of publishes and current total usage.
     */
    void updatePublishInfo_();
public:

    /**
     * @brief Currently only initializes private members with defined macros. 
     * Stub for future functionality. 
     */
    void initialize();

    /**
     * @brief Calls the Particle publish method based on time and data restrictions
     * 
     * @param String pumpId: Stringified integer representing the pump ID
     * @param String message: String containing data to be published by the Particle
     */
    void Publish(String pumpId, String message);
};