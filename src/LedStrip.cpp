#include "LedStrip.h"

NeoTopology<ColumnMajorAlternating180Layout> topo = NeoTopology<ColumnMajorAlternating180Layout>(MATRIX_WIDTH, MATRIX_HEIGHT);

uint16_t remap(uint16_t x, uint16_t y) {
  return topo.Map(x, y);
}

LedStrip::LedStrip() : matrix(NeoPixelBrightnessBusGfx<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>(MATRIX_WIDTH, MATRIX_HEIGHT, LED_STRIP_PIN))
{
}

LedStrip::~LedStrip()
{
}

void LedStrip::loadModule(Module* module) {
  this->activeModule = module;

  activeModule->setup();
}

void LedStrip::setup(int fps)
{
  this->fps = fps;
  matrix.Begin();

  // pass the remap function
  matrix.setRemapFunction(&remap);
  matrix.SetBrightness(40);
}

boolean LedStrip::_shouldDo()
{
  unsigned long currentMillis = millis();
  if (matrix.CanShow() && (fps <= -1 || currentMillis - previousMillis >= 1000 / fps))
  {
    previousMillis = currentMillis;
    return true;
  }

  return false;
}

void LedStrip::setLed(uint16_t num, RgbColor color)
{
  matrix.SetPixelColor(num, color);
}

void LedStrip::setLed(uint16_t x, uint16_t y, RgbColor color)
{
  matrix.SetPixelColor(remap(x, y), color);
}

void LedStrip::setBrightness(uint8_t brightness)
{
  matrix.SetBrightness(brightness);
}

void LedStrip::loop()
{
  if (_shouldDo()) {
    activeModule->loop();
  }
}