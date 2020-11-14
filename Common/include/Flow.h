/*
 * Content: reads water flow sensor and creates packets, returns string and package
 * Author : Ryan Leung
 * Team   : Sustaingineering UBC
 */
 
#pragma once
#include <Arduino.h>
#include "farmSensor.h"
 
class Flow : public farmSensor
{ 
  protected:
      float readRaw() override;
  public:
      Flow(int pin, String name, String unit, char shortcut);
      void initialize() override;
};
