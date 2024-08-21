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

void Display::error(const char *message)
{
    iconWithText(red, white, bitmap_error, message);
}

void Display::warning(const char *message)
{
    iconWithText(orange, white, bitmap_warning, message);
}

void Display::iconWithText(uint16_t backgroundColor, uint16_t foregroundColor, const byte *bitmap, const char *message)
{
    display->fillScreen(backgroundColor);

    uint16_t x = (display->width() - ICON_WIDTH) / 2;
    uint16_t y = display->height() / 2 - ICON_HEIGHT - 16;
    display->drawBitmap(x, y, bitmap, ICON_WIDTH, ICON_HEIGHT, foregroundColor);

    display->setTextColor(foregroundColor);
    display->setTextWrap(false);
    display->setTextSize(3);

    Bounds textBounds = getTextBounds(message);

    display->setCursor(textBounds.centerOrigin.x, textBounds.centerOrigin.y + textBounds.height);
    display->print(message);
}

Bounds Display::getTextBounds(const char *message)
{
    int16_t x1, y1;
    uint16_t width, height;

    display->getTextBounds(message, 0, 0, &x1, &y1, &width, &height);

    int16_t x = (display->width() - width) / 2;
    int16_t y = (display->height() - height) / 2;

    return {width, height, {x, y}};
}
