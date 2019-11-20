#include "Mqtt.h"
#include "Util.h"
#include <functional>

#define TOPIC "/led"
#define TOPIC_ALL TOPIC "/#"
#define TOPIC_SET_BRIGHTNESS TOPIC "/set/brightness"
#define TOPIC_SET_FPS TOPIC "/set/fps"

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
    String topicString = String(topic);

    // first check global settings
    if (topicString.equals(TOPIC_SET_BRIGHTNESS)) {
        if (Util::isValidNumber(message)) {
            int brightness = message.toInt();
            if (brightness >= 0 && brightness <= 255) {
                led->setBrightness(brightness);
            } else {
                Serial.println("the brightness has to be an integer from 0 to 255");
            }
        }
        return;
    } else if (topicString.equals(TOPIC_SET_FPS)) {
        // -1 is fps disabled -> as fast as it can
        if (Util::isValidNumber(message)) {
            int fps = message.toInt();
            if (fps >= -1 && fps <= 500) {
                led->setFps(fps);
                
            } else {
                Serial.println("the fps has to be an integer from -1 to 500");
            }
        }
        return;
    }

    // then check modules
    int baseTopicLength = strlen(TOPIC) + strlen(textModule.getModuleTopic());
    if(topicString.startsWith(String(TOPIC) + String(textModule.getModuleTopic()))) {
        if (textModule.processMqtt(topicString.substring(baseTopicLength), message)) {
            return;
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