#ifndef MQTT_H_
#define MQTT_H_

#include "Arduino.h"
#include "Web.h"
#include "LedStrip.h"
#include "Config.h"
#include "Modules/TextModule.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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

    WiFiClient espClient;
    PubSubClient client;

    TextModule textModule;

    void _receiveCallback(char* topic, byte* payload, unsigned int length);
    void _reconnect();
    
    static String _byte2str(byte* data, unsigned int length);
    static boolean _isValidNumber(String str);
};

#endif /* MQTT_H_ */