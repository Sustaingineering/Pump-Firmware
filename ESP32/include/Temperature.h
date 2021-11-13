#include "FarmSensor.h"
#include <OneWire.h> 
#include <DallasTemperature.h>

class Temperature : public FarmSensor
{
private:
    OneWire m_oneWire;
    DallasTemperature m_sensors;
protected:
    float readRaw() override;
public:
    Temperature(int pin, sensorType type, String name, String unit, char shortcut);
    bool initialize() override;
};
