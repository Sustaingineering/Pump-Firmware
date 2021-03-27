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
    bool initialize() override;
};

Temperature::Impl::Impl(int pin, sensorType type, String name, String unit, char shortcut): 
                FarmSensor(pin, type, name, unit, shortcut), m_oneWire(pin), m_sensors(&m_oneWire) {}

bool Temperature::Impl::initialize()
{
    m_sensors.begin();

    m_sensors.requestTemperatures();

    float temp = m_sensors.getTempCByIndex(0);

    if ((int) temp == DEVICE_DISCONNECTED_C)
        isWorking = false;
    else
        isWorking = true;

    return isWorking;
}

float Temperature::Impl::readRaw()
{
    if (isWorking == false)
        return nanf("0");

    m_sensors.requestTemperatures();

    float temp = m_sensors.getTempCByIndex(0);

    if ((int) temp == DEVICE_DISCONNECTED_C)
    {    
        Serial.print("TEMPERATURE SENSOR IS DISCONNECTED\n");
        isWorking = false;
        return nanf("0");
    }
    else
        return temp;
}

Temperature::Temperature(int pin, sensorType type, String name, String unit, char shortcut):
    m_pImpl(new Temperature::Impl(pin, type, name, unit, shortcut))
{}

bool Temperature::initialize() { return m_pImpl->initialize(); }

String Temperature::read() { return m_pImpl->read(); }

packet Temperature::pack() { return m_pImpl->pack(); }
