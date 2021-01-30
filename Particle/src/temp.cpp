#include "temp.h"
#include "OneWire.h"
#include "DS18.h"

class temp::Impl: public FarmSensor
{
private:
    DS18 m_sensor;
protected:
    float readRaw() override;
public:
    Impl(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize() override;
};

temp::Impl::Impl(int pin, sensorType type, String name, String unit, char shortcut): 
                FarmSensor(pin, type, name, unit, shortcut), m_sensor(pin){}

void temp::Impl::initialize()
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

float temp::Impl::readRaw()
{
    while (!m_sensor.read());
    return m_sensor.celsius();
}

temp::temp(int pin, sensorType type, String name, String unit, char shortcut):
    m_pImpl(new temp::Impl(pin, type, name, unit, shortcut))
{}

void temp::initialize() { m_pImpl->initialize(); }

String temp::read() { return m_pImpl->read(); }

packet temp::pack() { return m_pImpl->pack(); }
