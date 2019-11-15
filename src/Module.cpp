#include "Module.h"
#include <Arduino.h>

void Module::setup() {
  this->doSetup();
}

void Module::loop() {
  this->doLoop();
}