#include "TextModule.h"
#include "Util.h"

TextModule::TextModule(LedStrip* led)
    :LedModule(led)
{
    this->setText(std::vector<String>{"Servus"});
}

TextModule::~TextModule() {}

void TextModule::addText(String text) {
    this->texts.push_back(text);
}

void TextModule::setText(std::vector<String> newTexts) {
    this->texts = newTexts;
}

bool TextModule::doNextDiscoveryMessage(uint8 i, const char* baseTopic, DynamicJsonDocument *doc, char type[], char objectName[]) {
    strcpy(type, "text");
    strcpy(objectName, "led_matrix");
    strcat(objectName, "_text");

    (*doc)["name"] = "LED Matrix Text";

    char textTopic[128];
    strcpy(textTopic, baseTopic);
    strcat(textTopic, this->doGetModuleTopic());
    strcat(textTopic, TOPIC_SET_TEXT);
    (*doc)["command_topic"] = textTopic;

    (*doc)["retain"] = true;
    (*doc)["icon"] = "mdi:format-color-text";
    return false;
}

bool TextModule::doProcessMqtt(String topic, String message) {
    if (topic.equals(TOPIC_SET_TEXT)) {
        this->setText(std::vector<String>{message});
        return true;
    } else if (topic.equals(TOPIC_ADD_TEXT)) {
        this->addText(message);
        return true;
    }

    return false;
}

const char* TextModule::doGetModuleTopic() {
    return "/text";
}

void TextModule::doSetup() {
    led->matrix.setTextWrap(false);
}

void TextModule::doLoop() {
    led->matrix.fillScreen(0);
    led->matrix.setCursor(x, 0);
    led->matrix.print(this->texts[currentTextNum]);
    if(--x < -(int)(6*this->texts[currentTextNum].length())) {
        x = led->matrix.width();
        if (++currentTextNum >= this->texts.size()) {
            currentTextNum = 0;
        }
        led->matrix.setTextColor(led->getNextColor());
    }
    led->matrix.Show();
}