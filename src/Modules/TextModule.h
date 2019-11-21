#ifndef TEXTMODULE_H_
#define TEXTMODULE_H_

#include "Arduino.h"
#include "LedStrip.h"
#include "LedModule.h"
#include <vector>

#define TOPIC_SET_TEXT "/text"
#define TOPIC_ADD_TEXT "/text/add"
#define TOPIC_SET_TEXT_COLOR "/color"
#define TOPIC_ADD_TEXT_COLOR "/color/add"

class TextModule: public LedModule {
public:
    TextModule(LedStrip* led);
    virtual ~TextModule();

    void addText(String text);
    void setText(std::vector<String> texts);
    void addColor(uint16_t color);
    void setColors(std::vector<uint16_t> colors);
private:
    std::vector<uint16_t> colors;
    uint8_t currentColorNum = 0;

    std::vector<String> texts;
    uint8_t currentTextNum = 0;

    int x = MATRIX_WIDTH;

    virtual bool doProcessMqtt(String topic, String message) override;
    virtual const char* doGetModuleTopic() override;
    virtual void doLoop() override;
    virtual void doSetup() override;
};

#endif /* TEXTMODULE_H_ */