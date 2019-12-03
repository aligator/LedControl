#ifndef MQTT_H_
#define MQTT_H_

#include "Arduino.h"
#include "Web.h"
#include "LedStrip.h"
#include "Config.h"
#include "Modules/TextModule.h"
#include "Modules/FullBlinkModule.h"
#include "Module.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <vector>

class Mqtt {
public:
    Mqtt(LedStrip* led);
	virtual ~Mqtt();

	void setup();
    void loop();

private:
    LedStrip* led;
    const char* broker = (char*) MQTT_BROKER;
    const uint port = MQTT_PORT;
    const char* user = (char*) MQTT_USER;
    const char* password = (char*) MQTT_PASSWD;

    WiFiClient espClient;
    PubSubClient client;

    std::vector<Module*> modules;

    void _receiveCallback(char* topic, byte* payload, unsigned int length);
    void _reconnect();
    
    static String _byte2str(byte* data, unsigned int length);
};

#endif /* MQTT_H_ */