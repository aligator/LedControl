#include "Module.h"
#include <Arduino.h>
#include <PubSubClient.h>

Module::~Module()
{
}

bool Module::nextDiscoveryMessage(uint8 i, const char* baseTopic, DynamicJsonDocument *doc, char type[], char objectName[]) {
  return this->doNextDiscoveryMessage(i, baseTopic, doc, type, objectName);
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