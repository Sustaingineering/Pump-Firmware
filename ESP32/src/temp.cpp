#include "temp.h"
#include <OneWire.h> 
#include <DallasTemperature.h>

class temp::Impl: public farmSensor
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

temp::Impl::Impl(int pin, sensorType type, String name, String unit, char shortcut): 
                farmSensor(pin, type, name, unit, shortcut), m_oneWire(pin), m_sensors(&m_oneWire) {}

void temp::Impl::initialize()
{
    m_sensors.begin();
}

float temp::Impl::readRaw()
{
    m_sensors.requestTemperatures();
    return m_sensors.getTempCByIndex(0);
}

temp::temp(int pin, sensorType type, String name, String unit, char shortcut):
    m_pImpl(new temp::Impl(pin, type, name, unit, shortcut))
{}

void temp::initialize() { m_pImpl->initialize(); }

String temp::read() { return m_pImpl->read(); }

packet temp::pack() { return m_pImpl->pack(); }
