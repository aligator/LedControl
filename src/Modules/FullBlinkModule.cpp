#include "FullBlinkModule.h"
#include <ArduinoJson.h>

FullBlinkModule::FullBlinkModule(LedStrip* led)
    :LedModule(led)
{
    led->setColors(std::vector<uint16_t> {
        led->matrix.Color(255, 0, 0)
    });
}

FullBlinkModule::~FullBlinkModule() {}

bool FullBlinkModule::doProcessMqtt(String topic, String message) {
    return false;
}

const char* FullBlinkModule::doGetModuleTopic() {
    return "/blink";
}

void FullBlinkModule::doSetup() {
}

void FullBlinkModule::doLoop() {
    if (currentlyOn) {
        led->matrix.fillScreen(0);
    } else {
        led->matrix.fillScreen(this->led->getNextColor());
    }

    led->matrix.Show();
}