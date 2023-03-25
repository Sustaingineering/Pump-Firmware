#include "Temperature.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_TEMPERATURE_SWITCH

/***********************************************************************************************************************************
 * Original Version of the temperature sensor
***********************************************************************************************************************************/
Temperature::Temperature(bool isConnected, int pin): 
                FarmSensor(isConnected, pin), m_sensor(pin){}

bool Temperature::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }
    
    int tries = 20;
    while (!m_sensor.read())
    {
        tries--;
        if (tries == 0)
        {
            LOGGER("ERROR: Temperature sensor failed to intitialize");
            isWorking = false;
            return false;
        }
    }
    LOGGER("Initialized temperature sensor");
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
    //         LOGGER("ERROR: CANNOT READ TEMPERATURE");
    
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

// Temperature::Temperature(bool isConnected, int pin): 
//                 FarmSensor(isConnected, pin), oneWire(pin), m_sensor(&oneWire){}

// bool Temperature::initialize()
// {
//     if (!m_isConnected)
//     {
//         return true;
//     }
    
//     int tries = 20;
//     do 
//     {
//         tries--;
//         if (tries == 0)
//         {
//             LOGGER("ERROR: Temperature sensor failed to intitialize");
//             isWorking = false;
//             return false;
//         }
//         m_sensor.requestTemperatures();
//     } while (!m_sensor.getTempCByIndex(0));
//     LOGGER("Initialized temperature sensor");
//     isWorking = true;
//     return isWorking;
// }

// float Temperature::readRaw()
// {
//     if (!m_isConnected)
//     {
//         return count();
//     }
    
//     if (isWorking == false)
//         return nanf("0");

//     return m_sensor.getTempC(0);
// }

// bool Temperature::checkConnection()
// {
//     uint8_t addr[8];

//     m_sensor.getAddress(addr, 0);
    
//     // Address will be all zero if device was not found or search is done 
//     for (int i = 0; i < 8; i++)
//         if (addr[i] != 0)
//             return true;

//     return false;
// }