#include "Gsm.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_GSM_SWITCH

Gsm::Gsm(bool isConnected)
{
    m_isConnected = isConnected;
    if (m_isConnected)
    {
        SYSTEM_MODE(AUTOMATIC)
    }
    else
    {
        SYSTEM_MODE(MANUAL)
    }
}

bool Gsm::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }

    delimiter = ";";
    m_buffer = "";
    m_counter = 0;
    LOGGER("Time between messages: " + String(TIME_BTWN_MESSAGES));
    LOGGER("Total messages cap: " + String(TOTAL_MESSAGES_CAP));
    LOGGER("Initialized GSM");
    return true;
}

String Gsm::Publish(String pumpId, String message)
{
    if (!m_isConnected)
    {
        return "";
    }
    /**
     * 1. If buffer is empty, add to it & start tracking time 
     * 2. If message time < MAXTIME_BTWN_MESSAGES; reject message; else add to buffer
     * 3. If m_counter == TOTAL_MESSAGES_CAP; publish & return message; else return empty string
     * 
     * delimiter = "\n"
     */
    int time = Time.now();

    if (m_buffer == "")
    {
        // Add to empty buffer & set up counter & timer
        m_buffer += message;
        m_buffer += delimiter;
        m_counter++;
        // time at which message was last added to the buffer
        m_timeFromLastMessage = Time.now();

    }
    else if (time - m_timeFromLastMessage >= TIME_BTWN_MESSAGES)
    {
        m_buffer += message;
        m_buffer += delimiter;
        m_counter++;
        m_timeFromLastMessage = time;

        if (m_counter == TOTAL_MESSAGES_CAP)
        {
            // Publish method may block (20 secs - 10 mins)
            // https://docs.particle.io/reference/device-os/firmware/#particle-publish-
            if (Particle.publish(pumpId, m_buffer.c_str()))
            {
                LOGGER("Succesfully Published buffer: " + m_buffer);
                String result = m_buffer;
                m_buffer = "";
                m_counter = 0;
                return result;
            }
            else
            {
                LOGGER("Could not publish message!");
                // Should probably do more here
            }
        }
    }
    LOGGER("Number of messages in buffer: " + String(m_counter));
    return String("");
}

int Gsm::getTotalDataUsage_()
{
    CellularData data;
    if (!Cellular.getDataUsage(data)) {
        LOGGER("Error! Not able to get data.");
        return 0;
    }
    else {
        LOGGER(data);
        return data.tx_total + data.rx_total;
    }
}
