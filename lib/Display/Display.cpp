#include "Display.h"

Display::Display()
{
}

bool Display::begin()
{
    if (display == nullptr)
    {
        display = new Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

        display->init(240, 240);  // Initialize ST7789 screen
        pinMode(3, INPUT_PULLUP); // RESET fix

        // Default rotation to align it with the carrier
        display->setRotation(2);
        display->fillScreen(black);
    }

    return true;
}

void Display::end()
{
    // delete display;
    display = nullptr;
}
