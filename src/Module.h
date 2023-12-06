#ifndef MODULE_H_
#define MODULE_H_

#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * Module is the subclass for all modules.
 * Modules define different operating modes.
 * For example the TextModule is for displaying text.
 */
class Module {
public:
    virtual ~Module();

    bool processMqtt(String topic, String message);
    bool nextDiscoveryMessage(uint8 i, const char* baseTopic, DynamicJsonDocument *doc, char type[], char objectName[]);
    const char* getModuleTopic();
    void loop();
    void setup();
private:
    /**
     * doProcessMqtt is called if a mqtt message is received and it starts by the base topic.
     * The topic is already without the base topic. So if the topic is /led/text/set/brightness,
     * the main base topic is /led and the topic for the module is /text then you will get only /set/brightness
     */
    virtual bool doProcessMqtt(String topic, String message) = 0;
    virtual bool doNextDiscoveryMessage(uint8 i, const char* baseTopic, DynamicJsonDocument *doc, char type[], char objectName[]) = 0;
    virtual const char* doGetModuleTopic() = 0;
    virtual void doLoop() = 0;
    virtual void doSetup() = 0;
};

#endif /* MODULE_H_ */