/*
 * Content: reads water flow sensor and creates packets, returns string and package
 * Author : Ryan Leung
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>
#include "FarmSensor.h"
 
#define FLOW_RATE_FACTOR 5.5 // * Q(L/min)

class Flow : public FarmSensor
{ 
  protected:
      float readRaw() override;
  public:
      Flow(bool isConnected, int pin);
      bool initialize() override;
};
