#include "TextModule.h"
#include <ArduinoJson.h>
#include "Util.h"

TextModule::TextModule(LedStrip* led)
    :LedModule(led)
{
    this->setText("Servus");
    this->setColors(std::vector<uint16_t>{
        led->matrix.Color(255, 0, 0),
        led->matrix.Color(0, 255, 0),
        led->matrix.Color(0, 0, 255)
    });
}

TextModule::~TextModule()
{
    if (this->colors != NULL) {
        free(this->colors);
    }
}

void TextModule::setText(String text) {
    this->text = text;
}

void TextModule::setColors(std::vector<uint16_t> newColors) {
    if (this->colors != NULL) {
        free(this->colors);
    }

    this->colorNum = newColors.size();

    this->colors = (uint16_t *) malloc(newColors.size() * sizeof(uint16_t));

    for(std::vector<uint16_t>::size_type i = 0; i != newColors.size(); i++) {  
        this->colors[i] = newColors[i];
    }
}

bool TextModule::doProcessMqtt(String topic, String message) {
    if (topic.equals(TOPIC_SET_TEXT)) {
        this->setText(message);
        return true;
    } else if (topic.equals(TOPIC_SET_TEXT_COLOR)) {
        // e.g. only on color using th numeric value or "[[255,0,0],[0,0,255]]"

        if (Util::isValidNumber(message)) {
            int color = message.toInt();
            if (color >= 0 && color <= 0xFFFFFF) {
                color = led->matrix.Color((color & 0xFF0000) >> 16,
                                            (color & 0x00FF00) >> 8,
                                            color & 0x0000FF);

                this->setColors(std::vector<uint16_t>{(uint16_t)color});
            } else {
                Serial.println("The color has to be an integer from 0 to 16777215");
            }
            return true;
        }

        // for 9*JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(9);
        StaticJsonDocument<560> doc = StaticJsonDocument<560>();
        deserializeJson(doc, message);

        std::vector<uint16_t> colors = std::vector<uint16_t>();

        if (doc.is<JsonArray>()) {
            JsonArray colorArray=doc.as<JsonArray>();

            for (JsonVariant colorVariant : colorArray) {
                if (!colorVariant.is<JsonArray>()) {
                    Serial.println("the colors have to be arrays of 3 values (r, g, b)");
                    return true;
                }

                JsonArray color = colorVariant.as<JsonArray>();
                if (color.size() == 3) {
                    if(color[0].is<short>() && (short)color[0] >= 0 && (short)color[0] <= 255 &&
                        color[1].is<short>() && (short)color[1] >= 0 && (short)color[1] <= 255 &&
                        color[2].is<short>() && (short)color[2] >= 0 && (short)color[2] <= 255) {

                        short r = color[0];
                        short g = color[1];
                        short b = color[2];
                        
                        colors.push_back(led->matrix.Color(r, g, b));
                    } else {
                        Serial.println("the color values have to be integers from 0 to 255");
                    }
                } else {
                    Serial.println("the colors have to be arrays of 3 values (r, g, b)");
                }
            }

            this->setColors(colors);
        } else {
            Serial.println("you have to pass an array of color-arrays or one color as integer");
        }

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
    led->matrix.print(this->text);
    if(--x < -(int)(6*this->text.length())) {
        x = led->matrix.width();
        if(++pass >= colorNum) {
            pass = 0;
        }
        led->matrix.setTextColor(colors[pass]);
    }
    led->matrix.Show();
}