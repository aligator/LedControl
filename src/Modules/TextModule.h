#ifndef TEXTMODULE_H_
#define TEXTMODULE_H_

#include "Arduino.h"
#include "LedStrip.h"
#include "LedModule.h"
#include <vector>

#define TOPIC_SET_TEXT "/text"
#define TOPIC_SET_TEXT_COLOR "/color"

class TextModule: public LedModule {
public:
    TextModule(LedStrip* led);
    virtual ~TextModule();

    void setText(String text);
    void setColors(std::vector<uint16_t> colors);
private:
    uint16_t* colors;
    uint8_t colorNum;

    String text;
    int x = MATRIX_WIDTH;
    int pass = 0;

    virtual bool doProcessMqtt(String topic, String message) override;
    virtual const char* doGetModuleTopic() override;
    virtual void doLoop() override;
    virtual void doSetup() override;
};

#endif /* TEXTMODULE_H_ */