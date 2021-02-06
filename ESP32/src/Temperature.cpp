#include "Temperature.h"
#include <OneWire.h> 
#include <DallasTemperature.h>

class Temperature::Impl: public FarmSensor
{
private:
    OneWire m_oneWire;
    DallasTemperature m_sensors;
protected:
    float readRaw() override;
public:
    Impl(int pin, sensorType type, String name, String unit, char shortcut);
    void initialize() override;
};

Temperature::Impl::Impl(int pin, sensorType type, String name, String unit, char shortcut): 
                FarmSensor(pin, type, name, unit, shortcut), m_oneWire(pin), m_sensors(&m_oneWire) {}

void Temperature::Impl::initialize()
{
    m_sensors.begin();
}

float Temperature::Impl::readRaw()
{
    m_sensors.requestTemperatures();
    return m_sensors.getTempCByIndex(0);
}

Temperature::Temperature(int pin, sensorType type, String name, String unit, char shortcut):
    m_pImpl(new Temperature::Impl(pin, type, name, unit, shortcut))
{}

void Temperature::initialize() { m_pImpl->initialize(); }

String Temperature::read() { return m_pImpl->read(); }

packet Temperature::pack() { return m_pImpl->pack(); }
