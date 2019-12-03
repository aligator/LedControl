#ifndef FULLBLINKMODULE_H_
#define FULLBLINKMODULE_H_

#include "Arduino.h"
#include "LedStrip.h"
#include "LedModule.h"
#include <vector>

#define TOPIC_SET_BLINK_COLOR "/color"
#define TOPIC_ADD_BLINK_COLOR "/color/add"

class FullBlinkModule: public LedModule {
public:
    FullBlinkModule(LedStrip* led);
    virtual ~FullBlinkModule();

    void addColor(uint16_t color);
    void setColors(std::vector<uint16_t> colors);
private:
    std::vector<uint16_t> colors;
    uint8_t currentColorNum = 0;

    bool currentlyOn = false;

    virtual bool doProcessMqtt(String topic, String message) override;
    virtual const char* doGetModuleTopic() override;
    virtual void doLoop() override;
    virtual void doSetup() override;
};

#endif /* FULLBLINKMODULE_H_ */