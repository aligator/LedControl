#include "TextModule.h"
#include "Util.h"

TextModule::TextModule(LedStrip *led)
    : LedModule(led)
{
    this->setText(std::vector<String>{"Servus"});
}

TextModule::~TextModule() {}

void TextModule::addText(String text)
{
    this->texts.push_back(text);
}

void TextModule::setText(std::vector<String> newTexts)
{
    this->texts = newTexts;
}

bool TextModule::doNextDiscoveryMessage(uint8 i, const char *baseTopic, DynamicJsonDocument *doc, char type[], char objectName[])
{
    switch (i)
    {
    case 0:
        strcpy(type, "text");
        strcpy(objectName, "led_matrix");
        strcat(objectName, "_text");

        (*doc)["name"] = "LED Matrix Text";

        char textTopic[128];
        strcpy(textTopic, baseTopic);
        strcat(textTopic, this->doGetModuleTopic());
        strcat(textTopic, TOPIC_TEXT);
        (*doc)["command_topic"] = textTopic;

        (*doc)["retain"] = true;
        (*doc)["icon"] = "mdi:format-color-text";
        return true;
    case 1:
        strcpy(type, "switch");
        strcpy(objectName, "led_matrix");
        strcat(objectName, "_scroll");

        (*doc)["name"] = "LED Matrix Text Scroll";

        char scrollTopic[128];
        strcpy(scrollTopic, baseTopic);
        strcat(scrollTopic, this->doGetModuleTopic());
        strcat(scrollTopic, TOPIC_SCROLL);
        (*doc)["command_topic"] = scrollTopic;
        (*doc)["state_topic"] = scrollTopic;

        (*doc)["retain"] = true;
        (*doc)["icon"] = "mdi:format-color-text";
        return false;
    default:
        return false;
    }
}

bool TextModule::doProcessMqtt(String topic, String message)
{
    if (topic.equals(TOPIC_TEXT))
    {
        this->setText(std::vector<String>{message});
        return true;
    }
    else if (topic.equals(TOPIC_SCROLL))
    {
        this->scroll = message.equals("ON");
        return true;
    }

    return false;
}

const char *TextModule::doGetModuleTopic()
{
    return "/text";
}

void TextModule::doSetup()
{
    led->matrix.setTextWrap(false);
}

void TextModule::doLoop()
{
    led->matrix.fillScreen(0);

    if (this->scroll)
    {
        led->matrix.setCursor(this->x, 0);
    }
    else
    {
        int pos = 0;
        // center text
        if (this->texts[currentTextNum].length() < static_cast<unsigned int>(led->matrix.width())/6)
        {
            pos = (led->matrix.width() - 6 * this->texts[currentTextNum].length()) / 2;
        }

        led->matrix.setCursor(pos, 0);
    }

    led->matrix.print(this->texts[currentTextNum]);

    if (--this->x < -(int)(6 * this->texts[currentTextNum].length()))
    {
        this->x = led->matrix.width();
        if (++currentTextNum >= this->texts.size())
        {
            currentTextNum = 0;
        }
        led->matrix.setTextColor(led->getNextColor());
    }
    led->matrix.Show();
}