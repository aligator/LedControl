#ifndef TEXTMODULE_H_
#define TEXTMODULE_H_

#include "Arduino.h"
#include "LedStrip.h"
#include "LedModule.h"
#include <list>

class TextModule: public LedModule {
public:
    TextModule(LedStrip* led);
    virtual ~TextModule();

    void setText(String text);
private:
    uint16_t colors[3] = {
        led->matrix.Color(255, 0, 0),
        led->matrix.Color(0, 255, 0),
        led->matrix.Color(0, 0, 255)
    };

    uint8_t colorNum;

    String text;
    int x = MATRIX_WIDTH;
    int pass = 0;

    virtual void doLoop() override;
    virtual void doSetup() override;
};

#endif /* TEXTMODULE_H_ */