#include "Temperature.h"

Temperature::Temperature(int pin, sensorType type, String name, String unit, char shortcut): 
                FarmSensor(pin, type, name, unit, shortcut), m_sensor(pin){}

bool Temperature::initialize()
{
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
