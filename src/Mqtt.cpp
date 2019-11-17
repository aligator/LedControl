#include "Mqtt.h"
#include <functional>
#include <ArduinoJson.h>

#define TOPIC "/led"
#define TOPIC_ALL TOPIC "/#"
#define TOPIC_SET_TEXT TOPIC "/set/text"
#define TOPIC_SET_BRIGHTNESS TOPIC "/set/brightness"
#define TOPIC_SET_FPS TOPIC "/set/fps"
#define TOPIC_SET_TEXT_COLOR TOPIC "/set/text/color"

boolean Mqtt::_isValidNumber(String str){
    boolean isNum=false;
    for(byte i=0; i<str.length(); i++)
    {
        isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
        if(!isNum) {
            return false;
        }
    }
    return isNum;
}

/**
 * converts bytes to a String
 */
String Mqtt::_byte2str(byte* data, unsigned int length) {
    char* buffer = (char*) malloc (length+1);
    memcpy(buffer, data, length);
    buffer[length] = 0;

    // or for unicode:
    //const char* str((const wchar_t*)buffer);
    String out = String((const char*) buffer);
    free(buffer);
    return out;
}

void Mqtt::_receiveCallback(char* topic, byte* payload, unsigned int length) {
    String message = Mqtt::_byte2str(payload, length);
    
    if (strcmp(topic, TOPIC_SET_TEXT) == 0) {
        textModule.setText(message);
        //led->loadModule(&textModule);
    } else if (strcmp(topic, TOPIC_SET_BRIGHTNESS) == 0) {
        if (Mqtt::_isValidNumber(message)) {
            int brightness = message.toInt();
            if (brightness >= 0 && brightness <= 255) {
                led->setBrightness(brightness);
            } else {
                Serial.println("the brightness has to be a integer from 0 to 255");
            }
        }
    } else if (strcmp(topic, TOPIC_SET_FPS) == 0) {
        // -1 is fps disabled -> as fast as it can
        if (Mqtt::_isValidNumber(message)) {
            int fps = message.toInt();
            if (fps >= -1 && fps <= 500) {
                led->setFps(fps);
            } else {
                Serial.println("the fps has to be a integer from -1 to 500");
            }
        }
    } else if (strcmp(topic, TOPIC_SET_TEXT_COLOR) == 0) {
        // e.g. "[[255,0,0],[0,0,255]]"

        // for 9*JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(9);
        StaticJsonDocument<560> doc = StaticJsonDocument<560>();
        deserializeJson(doc, message);

        std::vector<uint16_t> colors = std::vector<uint16_t>();

        if (doc.is<JsonArray>()) {
            JsonArray colorArray=doc.as<JsonArray>();

            for (JsonVariant colorVariant : colorArray) {
                if (!colorVariant.is<JsonArray>()) {
                    Serial.println("the colors have to be arrays of 3 values (r, g, b)");
                    return;
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

            textModule.setColors(colors);
        } else {
            Serial.println("you have to pass an array");
        }
    }
}

Mqtt::Mqtt(LedStrip* led): textModule(TextModule(led)) {
    this->led = led;
}

Mqtt::~Mqtt() {
}

void Mqtt::setup() {
    delay(10);
    this->client = PubSubClient(broker, port, espClient);
    this->client.setCallback(std::bind(&Mqtt::_receiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    this->led->loadModule(&textModule);
}

void Mqtt::_reconnect() {
    if (!client.connected()) {
        Serial.print("Reconnecting...");

        String clientId = "LED-";
        clientId += String(random(0xffff), HEX);

        // Attempt to connect
        if (!client.connect(clientId.c_str(), user, password)) {
            Serial.print("failed, rc=");
            Serial.println(client.state());
        } else {
            Serial.println("Connected");
            delay(100);
            client.subscribe(TOPIC_ALL);
        }
    }
}

void Mqtt::loop() {
    if (!client.connected()) {
        _reconnect();
    } else {
        
        client.loop();
        
    }
}