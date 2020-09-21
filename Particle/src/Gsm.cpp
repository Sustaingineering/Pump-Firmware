#include "Gsm.h"

void Gsm::Publish(String pumpId, String message)
{
    int now = (int)Time.now();
    if (now - m_lastTimePublished >= 10)
    {
        Particle.publish(pumpId.c_str(), message.c_str(), PRIVATE);
        m_lastTimePublished = now;
        Serial.println("Particle Published!");
    }
}