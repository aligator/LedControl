#include "FullBlinkModule.h"
#include <ArduinoJson.h>
#include "Util.h"

FullBlinkModule::FullBlinkModule(LedStrip* led)
    :LedModule(led)
{
    this->setColors(std::vector<uint16_t>{
        led->matrix.Color(255, 0, 0),
        led->matrix.Color(0, 255, 0),
        led->matrix.Color(0, 0, 255)
    });
}

FullBlinkModule::~FullBlinkModule()
{
}

void FullBlinkModule::addColor(uint16_t color) {
    this->colors.push_back(color);
}

void FullBlinkModule::setColors(std::vector<uint16_t> newColors) {
    this->colors = newColors;
}

bool FullBlinkModule::doProcessMqtt(String topic, String message) {
    // TODO: move color to led as it's the same for all modules
    if (topic.equals(TOPIC_ADD_BLINK_COLOR)) {
        // numeric value
        if (Util::isValidNumber(message)) {
            int color = message.toInt();
            if (color >= 0 && color <= 0xFFFFFF) {
                color = led->matrix.Color((color & 0xFF0000) >> 16,
                                            (color & 0x00FF00) >> 8,
                                            color & 0x0000FF);

                this->addColor((uint16_t)color);
            } else {
                Serial.println("The color has to be an integer from 0 to 16777215");
            }
            return true;
        }

        // json array
        // for JSON_ARRAY_SIZE(3);
        StaticJsonDocument<48> doc = StaticJsonDocument<48>();
        deserializeJson(doc, message);

        if (doc.is<JsonArray>()) {
            JsonArray color=doc.as<JsonArray>();

            if (color.size() == 3) {
                if(color[0].is<short>() && (short)color[0] >= 0 && (short)color[0] <= 255 &&
                    color[1].is<short>() && (short)color[1] >= 0 && (short)color[1] <= 255 &&
                    color[2].is<short>() && (short)color[2] >= 0 && (short)color[2] <= 255) {

                    short r = color[0];
                    short g = color[1];
                    short b = color[2];
                    
                    this->addColor(led->matrix.Color(r, g, b));
                } else {
                    Serial.println("the color values have to be integers from 0 to 255");
                }
            } else {
                Serial.println("the colors have to be arrays of 3 values (r, g, b)");
            }
        } else {
            Serial.println("you have to pass an array of color-arrays or one color as integer");
        }

        return true;

    } else if (topic.equals(TOPIC_SET_BLINK_COLOR)) {
        // e.g. only one color using th numeric value or "[[255,0,0],[0,0,255]]"

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

const char* FullBlinkModule::doGetModuleTopic() {
    return "/blink";
}

void FullBlinkModule::doSetup() {
}

void FullBlinkModule::doLoop() {
    if (currentlyOn) {
        led->matrix.fillScreen(0);
    } else {
        if (++currentColorNum >= this->colors.size()) {
            currentColorNum = 0;
        }
        led->matrix.fillScreen(this->colors[currentColorNum]);
    }

    led->matrix.Show();
}