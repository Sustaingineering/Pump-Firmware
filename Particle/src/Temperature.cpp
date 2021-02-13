#include "Temperature.h"
#include "OneWire.h"
#include "DS18.h"

class Temperature::Impl: public FarmSensor
{
private:
    DS18 m_sensor;
protected:
    float readRaw() override;
public:
    Impl(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize() override;
};

Temperature::Impl::Impl(int pin, sensorType type, String name, String unit, char shortcut): 
                FarmSensor(pin, type, name, unit, shortcut), m_sensor(pin){}

void Temperature::Impl::initialize()
{
    if (m_sensor.read())
    {
        Serial.println("Initialized!");
    }
    else
    {
        if (m_sensor.searchDone())
        {
            Serial.println("ERROR: No More Addresses");
        } 
        else
        {
            Serial.println("ERROR: Something Went Wrong!");
        }
    }
}

float Temperature::Impl::readRaw()
{
    while (!m_sensor.read());
    return m_sensor.celsius();
}

Temperature::Temperature(int pin, sensorType type, String name, String unit, char shortcut):
    m_pImpl(new Temperature::Impl(pin, type, name, unit, shortcut))
{}

void Temperature::initialize() { m_pImpl->initialize(); }

String Temperature::read() { return m_pImpl->read(); }

packet Temperature::pack() { return m_pImpl->pack(); }
