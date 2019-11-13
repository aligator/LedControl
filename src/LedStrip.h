#ifndef LED_STRIP_H_
#define LED_STRIP_H_

#include "Arduino.h"
#include "Config.h"

#include <NeoPixelBrightnessBusGfx.h>
#include <NeoPixelBrightnessBus.h>

#define NUMPIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

class LedStrip {
public:
    LedStrip();
	virtual ~LedStrip();

	void setup(int fps);
    void setLed(uint16_t num, RgbColor color);
    void setLed(uint16_t x, uint16_t y, RgbColor color);
    void setBrightness(uint8_t brightness);
    void showText();
    void setText(String text);

private:
    NeoPixelBrightnessBusGfx<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod> matrix;

    unsigned long previousMillis = 0;
    long fps = -1;

    int x = MATRIX_WIDTH;
    int pass = 0;
    
    String text;

    boolean _shouldDo();
};

#endif /* LED_STRIP_H_ */