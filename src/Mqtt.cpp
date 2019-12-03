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
    } else {
        // then check modules

        for (int i=0; i<this->modules.size(); i++) {
            int baseTopicLength = strlen(TOPIC) + strlen(this->modules[i]->getModuleTopic());
            if(topicString.startsWith(String(TOPIC) + String(this->modules[i]->getModuleTopic()))) {
                // enable module as soon as the topic starts with the module topic
                this->led->loadModule(this->modules[i]);
                if (this->modules[i]->processMqtt(topicString.substring(baseTopicLength), message)) {
                    return;
                }

                break;
            }
        }
    }

    
}

Mqtt::Mqtt(LedStrip* led) {
    this->led = led;
    this->modules = std::vector<Module*>{
        new TextModule(led),
        new FullBlinkModule(led)
    };
}

Mqtt::~Mqtt() {
    for (int i=0; i<this->modules.size(); i++) {
        this->led->loadModule(0);
        delete this->modules[i];
        this->modules.pop_back();
    }
}

void Mqtt::setup() {
    delay(10);
    this->client = PubSubClient(broker, port, espClient);
    this->client.setCallback(std::bind(&Mqtt::_receiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    if (this->modules.size() > 0){
        this->led->loadModule(this->modules[0]);
    }
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