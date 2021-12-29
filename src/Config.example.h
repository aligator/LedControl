#ifndef CONFIG_H_
#define CONFIG_H_

#define LED_STRIP_PIN 3

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8

// 0-255
#define MATRIX_MAX_BRIGHTNESS 100

#define MQTT_BROKER "192.168.178.53"
#define MQTT_PORT 1883
#define MQTT_USER "...."
#define MQTT_PASSWD "...."

#define MQTT_TOPIC "/led"

#define WIFI_SSID "...."
#define WIFI_PASSWD "...."
#define WIFI_HOSTNAME "arduino-pixel"

#define OTA_PASSWD "....."
#define OTA_PORT 8266

#endif /* CONFIG_H_ */