#include "Temperature.h"

Temperature::Temperature(bool isConnected, int pin): 
                FarmSensor(isConnected, pin), m_sensor(pin){}

bool Temperature::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }
    Serial.println("Initializing Temperature Sensor...");
    
    int tries = 20;
    while (!m_sensor.read())
    {
        tries--;
        if (tries == 0)
        {
            Serial.println("ERROR: Temperature sensor failed to intitialize");
            isWorking = false;
            return false;
        }
    }

    isWorking = true;
    return isWorking;
}

float Temperature::readRaw()
{
    if (!m_isConnected)
    {
        return count();
    }
    if (isWorking == false)
        return nanf("0");

    while (!m_sensor.read());
    // https://github.com/particle-iot/OneWireLibrary
    // {
    //     if (!checkConnection())
    //     {
    //         Serial.println("ERROR: CANNOT READ TEMPERATURE");
    
    //         isWorking = false;

    //         return nanf("0");
    //     }
    // }

    return m_sensor.celsius();
}

bool Temperature::checkConnection()
{
    uint8_t addr[8];

    m_sensor.addr(addr);
    
    // Address will be all zero if device was not found or search is done 
    for (int i = 0; i < 8; i++)
        if (addr[i] != 0)
            return true;

    return false;
}
