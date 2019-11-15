#include "TextModule.h"

TextModule::TextModule(LedStrip* led)
    :LedModule(led)
{
    this->text = String("Servus");
}

TextModule::~TextModule()
{
}

void TextModule::setText(String text) {
    this->text = text;
}

void TextModule::doSetup() {
    led->matrix.setTextWrap(false);
}

void TextModule::doLoop() {
    led->matrix.fillScreen(0);
    led->matrix.setCursor(x, 0);
    led->matrix.print(this->text);
    if(--x < -(int)(6*this->text.length())) {
        x = led->matrix.width();
        if(++pass >= 3) {
            pass = 0;
        }
        led->matrix.setTextColor(colors[pass]);
    }
    led->matrix.Show();
}