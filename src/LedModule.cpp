#include "LedModule.h"
#include <Arduino.h>

LedModule::LedModule(LedStrip* led)
{
  this->led = led;
}

LedModule::~LedModule()
{
}