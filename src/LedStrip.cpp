#include "LedStrip.h"

NeoTopology<ColumnMajorAlternating180Layout> topo = NeoTopology<ColumnMajorAlternating180Layout>(MATRIX_WIDTH, MATRIX_HEIGHT);

uint16_t remap(uint16_t x, uint16_t y) {
  return topo.Map(x, y);
}

LedStrip::LedStrip() : matrix(NeoPixelBrightnessBusGfx<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>(MATRIX_WIDTH, MATRIX_HEIGHT, LED_STRIP_PIN))
{
  this->setColors(std::vector<uint16_t>{
    this->matrix.Color(255, 0, 0),
    this->matrix.Color(0, 255, 0),
    this->matrix.Color(0, 0, 255)
  });
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

void LedStrip::addColor(uint16_t color) {
    this->colors.push_back(color);
}

void LedStrip::setColors(std::vector<uint16_t> newColors) {
    this->colors = newColors;
}

uint16_t LedStrip::getNextColor() {
  if (this->colors.size() > 0) {
    if (++currentColorNum >= this->colors.size()) {
      currentColorNum = 0;
    }
    return colors[currentColorNum];
  } else {
    return 0;
  }
}

uint16_t LedStrip::getCurrentColor() {
  if (this->colors.size() > 0) {
    return colors[currentColorNum];
  } else {
    return 0;
  }
}

boolean LedStrip::_shouldDo()
{
  unsigned long currentMillis = millis();
  if (fps != 0 && matrix.CanShow() && (fps <= -1 || currentMillis - previousMillis >= 1000 / (unsigned int)fps))
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

void LedStrip::setFps(int fps) {
  this->fps = fps;
}

void LedStrip::loop()
{
  if (_shouldDo()) {
    activeModule->loop();
  }
}