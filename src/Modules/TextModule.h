#ifndef TEXTMODULE_H_
#define TEXTMODULE_H_

#include "Arduino.h"
#include "LedStrip.h"
#include "LedModule.h"
#include <vector>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define TOPIC_SCROLL "/scroll"
#define TOPIC_TEXT "/text"

class TextModule: public LedModule {
public:
    TextModule(LedStrip* led);
    virtual ~TextModule();

    void addText(String text);
    void setText(std::vector<String> texts);
private:
    bool scroll = true;
    std::vector<String> texts;
    uint8_t currentTextNum = 0;

    int x = MATRIX_WIDTH;

    virtual bool doProcessMqtt(String topic, String message) override;
    virtual bool doNextDiscoveryMessage(uint8 i, const char* baseTopic, DynamicJsonDocument *doc, char type[], char objectName[]) override;
    virtual const char* doGetModuleTopic() override;
    virtual void doLoop() override;
    virtual void doSetup() override;
};

#endif /* TEXTMODULE_H_ */