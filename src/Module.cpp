#include "Module.h"
#include <Arduino.h>

Module::~Module()
{
}

bool Module::processMqtt(String topic, String message) {
  return this->doProcessMqtt(topic, message);
}

const char* Module::getModuleTopic() {
  return this->doGetModuleTopic();
}

void Module::setup() {
  this->doSetup();
}

void Module::loop() {
  this->doLoop();
}