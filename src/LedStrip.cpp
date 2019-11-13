#include "LedStrip.h"

NeoTopology<ColumnMajorAlternating180Layout> topo = NeoTopology<ColumnMajorAlternating180Layout>(MATRIX_WIDTH, MATRIX_HEIGHT);

uint16_t remap(uint16_t x, uint16_t y) {
  return topo.Map(x, y);
}

LedStrip::LedStrip() : matrix(NeoPixelBrightnessBusGfx<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>(MATRIX_WIDTH, MATRIX_HEIGHT, LED_STRIP_PIN))
{
  this->text = String("Servus");
}

LedStrip::~LedStrip()
{
}

void LedStrip::setText(String text) {
  this->text = text;
}

void LedStrip::setup(int fps)
{
  this->fps = fps;
  matrix.Begin();

  // pass the remap function
  matrix.setRemapFunction(&remap);

  matrix.setTextWrap(false);
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

void LedStrip::showText()
{
  if (this->_shouldDo()) {
    const uint16_t colors[] = {
      matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    matrix.print(this->text);
    if(--x < -(int)(6*this->text.length())) {
      x = matrix.width();
      if(++pass >= 3) pass = 0;
      matrix.setTextColor(colors[pass]);
    }

    matrix.Show();
  }
}