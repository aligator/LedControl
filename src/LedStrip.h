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

    bool enabled = false;

	void setup(float fps);
	void loop();
    void setLed(uint16_t num, RgbColor color);
    void setLed(uint16_t x, uint16_t y, RgbColor color);
    void setBrightness(uint8_t brightness);
    void setFps(float fps);
    void loadModule(Module* module);

    void addColor(uint16_t color);
    void setColors(std::vector<uint16_t> colors);
    uint16_t getNextColor();
    uint16_t getCurrentColor();

private:
    std::vector<uint16_t> colors;
    uint8_t currentColorNum = 0;

    unsigned long previousMillis = 0;
    float fps = -1;

    /**
     * Save if the "clear" needs to be executed.
     */
    bool enabledDirty = true;

    Module* activeModule;
    
    /**
     * returns true if the next image should be drawn. It is calculated by the fps;
     */
    boolean _shouldDo();
};

#endif /* LED_STRIP_H_ */