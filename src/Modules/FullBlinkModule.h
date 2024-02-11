#ifndef FULLBLINKMODULE_H_
#define FULLBLINKMODULE_H_

#include "Arduino.h"
#include "LedStrip.h"
#include "LedModule.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define TOPIC_BLINK_ENABLED "/enabled"

class FullBlinkModule: public LedModule {
public:
    FullBlinkModule(LedStrip* led);
    virtual ~FullBlinkModule();
    
private:
    bool enabled = false;
    bool currentlyOn = false;

    virtual bool doProcessMqtt(String topic, String message) override;
    virtual bool doNextDiscoveryMessage(uint8 i, const char* baseTopic, DynamicJsonDocument *doc, char type[], char objectName[]) override;
    virtual const char* doGetModuleTopic() override;
    virtual void doLoop() override;
    virtual void doSetup() override;
};

#endif /* FULLBLINKMODULE_H_ */