#include "Temperature.h"
#include "OneWire.h"
#include "DS18.h"

class Temperature::Impl: public FarmSensor
{
private:
    DS18 m_sensor;
    bool checkConnection();
protected:
    float readRaw() override;
public:
    Impl(int pin, sensorType type, String name, String unit, char shortcut);
    bool initialize() override;
};

Temperature::Impl::Impl(int pin, sensorType type, String name, String unit, char shortcut): 
                FarmSensor(pin, type, name, unit, shortcut), m_sensor(pin){}

bool Temperature::Impl::initialize()
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

float Temperature::Impl::readRaw()
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

bool Temperature::Impl::checkConnection()
{
    uint8_t addr[8];

    m_sensor.addr(addr);
    
    // Address will be all zero if device was not found or search is done 
    for (int i = 0; i < 8; i++)
        if (addr[i] != 0)
            return true;

    return false;
}

Temperature::Temperature(int pin, sensorType type, String name, String unit, char shortcut):
    m_pImpl(new Temperature::Impl(pin, type, name, unit, shortcut))
{}

bool Temperature::initialize() { return m_pImpl->initialize(); }

String Temperature::read() { return m_pImpl->read(); }

packet Temperature::pack() { return m_pImpl->pack(); }
