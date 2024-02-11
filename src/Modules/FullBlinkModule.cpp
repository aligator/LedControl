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

bool FullBlinkModule::doNextDiscoveryMessage(uint8 i, const char* baseTopic, DynamicJsonDocument *doc, char type[], char objectName[]) {
    switch (i)
    {
    case 0:
        strcpy(type, "switch");
        strcpy(objectName, "led_matrix");
        strcat(objectName, "_blink");

        (*doc)["name"] = "LED Matrix Blink";

        char blinkTopic[128];
        strcpy(blinkTopic, baseTopic);
        strcat(blinkTopic, this->doGetModuleTopic());
        strcat(blinkTopic, TOPIC_BLINK_ENABLED);
        (*doc)["command_topic"] = blinkTopic;
        (*doc)["state_topic"] = blinkTopic;

        (*doc)["retain"] = true;
        (*doc)["icon"] = "mdi:format-color-text";
        return false;
    default:
        return false;
    }
}

bool FullBlinkModule::doProcessMqtt(String topic, String message) {
    if (topic.equals(TOPIC_BLINK_ENABLED))
    {
        this->enabled = message.equals("ON");
        return true;
    }

    return false;
}

const char* FullBlinkModule::doGetModuleTopic() {
    return "/blink";
}

void FullBlinkModule::doSetup() {
}

void FullBlinkModule::doLoop() {
    if (!this->enabled) {
        return;
    }

    if (currentlyOn) {
        led->matrix.fillScreen(0);
    } else {
        led->matrix.fillScreen(this->led->getNextColor());
    }

    led->matrix.Show();
}