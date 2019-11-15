#include "Mqtt.h"
#include <functional>

#define TOPIC "/led"
#define TOPIC_ALL TOPIC "/#"
#define TOPIC_SET_TEXT TOPIC "/set/text"
#define TOPIC_SET_BRIGHTNESS TOPIC "/set/brightness"
#define TOPIC_SET_FPS TOPIC "/set/fps"

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
            }
        }
    } else if (strcmp(topic, TOPIC_SET_FPS) == 0) {
        if (Mqtt::_isValidNumber(message)) {
            int fps = message.toInt();
            if (fps >= -1 && fps <= 500) {
                led->setFps(fps);
            }
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
        if (!client.connect(clientId.c_str())) {
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