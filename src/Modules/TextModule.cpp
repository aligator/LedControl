#include "TextModule.h"

TextModule::TextModule(LedStrip* led)
    :LedModule(led)
{
    this->setText("Servus");
    this->setColors(std::vector<uint16_t>{
        led->matrix.Color(255, 0, 0),
        led->matrix.Color(0, 255, 0),
        led->matrix.Color(0, 0, 255)
    });
}

TextModule::~TextModule()
{
    if (this->colors != NULL) {
        free(this->colors);
    }
}

void TextModule::setText(String text) {
    this->text = text;
}

void TextModule::setColors(std::vector<uint16_t> newColors) {
    if (this->colors != NULL) {
        free(this->colors);
    }

    this->colorNum = newColors.size();

    this->colors = (uint16_t *) malloc(newColors.size() * sizeof(uint16_t));

    for(std::vector<uint16_t>::size_type i = 0; i != newColors.size(); i++) {  
        this->colors[i] = newColors[i];
    }
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
        if(++pass >= colorNum) {
            pass = 0;
        }
        led->matrix.setTextColor(colors[pass]);
    }
    led->matrix.Show();
}