#ifndef LED_STRIP_H_
#define LED_STRIP_H_

class Module;

#include "Arduino.h"
#include "Config.h"
#include "Module.h"

#include <NeoPixelBrightnessBusGfx.h>
#include <NeoPixelBrightnessBus.h>

#define NUMPIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

class LedStrip {
public:
    NeoPixelBrightnessBusGfx<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod> matrix;

    LedStrip();
	virtual ~LedStrip();

	void setup(int fps);
	void loop();
    void setLed(uint16_t num, RgbColor color);
    void setLed(uint16_t x, uint16_t y, RgbColor color);
    void setBrightness(uint8_t brightness);
    void loadModule(Module* module);

private:
    unsigned long previousMillis = 0;
    long fps = -1;

    Module* activeModule;
    
    /**
     * returns true if the next image should be drawn. It is calculated by the fps;
     */
    boolean _shouldDo();
};

#endif /* LED_STRIP_H_ */