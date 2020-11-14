#include "Gsm.h"


void Gsm::initialize()
{
    m_maxData0Bytes = MAX_DATA_0;
    m_publishPeriod = DEFAULT_PERIOD;
}

void Gsm::Publish(String pumpId, String message)
{
    int now = (int)Time.now();
    if (now - m_lastTimePublished >= m_publishPeriod)
    {
        updatePublishInfo_(); 

        Particle.publish(pumpId.c_str(), message.c_str(), PRIVATE);
        m_lastTimePublished = now;
        Serial.println("Particle Published!");

        if (m_numPublish == 2)
        {
            computePublishPeriod_();
        }
    } 
}

void Gsm::computePublishPeriod_()
{
    delay(5000);
    int messageLength = getTotalDataUsage_() - m_currentUsageBytes;
    Serial.printlnf("Message Length: %d bytes", messageLength);

    int numMessagesPerMonth = (MAX_DATA_LIMIT_BYTES - (m_maxData0Bytes * NUM_DAYS)) / messageLength;

    m_publishPeriod = NUM_DAYS * NUM_HOURS * SECONDS_IN_HOUR / numMessagesPerMonth;
    Serial.printlnf("Publish Period: %d seconds", m_publishPeriod);
}

void Gsm::updatePublishInfo_()
{
    m_numPublish++;
    m_currentUsageBytes = getTotalDataUsage_();
    Serial.printlnf("Current Total Data usage: %d bytes", m_currentUsageBytes);
}

int Gsm::getTotalDataUsage_()
{
    CellularData data;
    if (!Cellular.getDataUsage(data)) {
        Serial.println("Error! Not able to get data.");
        return 0;
    }
    else {
        Serial.println(data);
        return data.tx_total + data.rx_total;
    }
}
