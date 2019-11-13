#include <Arduino.h>

#include "Web.h"
#include "Mqtt.h"
#include "LedStrip.h"

// Replace with your network credentials
const char* ssid = WIFI_SSID;
const char* wiFiPassword = WIFI_PASSWD;
const char* hostname = WIFI_HOSTNAME;

const uint16_t otaPort = OTA_PORT;
const char* otaPassword = OTA_PASSWD;

Web* web = new Web(ssid, wiFiPassword, hostname, otaPort, otaPassword);
LedStrip* ledStrip = new LedStrip();
Mqtt* mqtt = new Mqtt(ledStrip);

void setup() {
  Serial.begin(9600);
  web->setup();
  mqtt->setup();
  
  ledStrip->setup(20);

  ledStrip->setBrightness(60);
}

void loop() {
  web->loop();
  mqtt->loop();

  ledStrip->showText();
}