#include "Display.h"

Display::Display()
{
}

Display::~Display()
{
    end();
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
        display->fillScreen(BLACK);

        // Set custom font
        display->setFont(&Org_01);
    }

    return true;
}

void Display::end()
{
    clear();
    display->enableDisplay(false);

    // delete display;
    display = nullptr;
}

void Display::clear()
{
    display->fillScreen(BLACK);
}

uint16_t Display::color565(uint8_t r, uint8_t g, uint8_t b)
{
    return display->color565(r, g, b);
}

void Display::error(String message)
{
    display->color565(255, 0, 0);
    iconWithText(RED, WHITE, bitmap_error, message);
}

void Display::warning(String message)
{
    iconWithText(ORANGE, WHITE, bitmap_warning, message);
}

void Display::drawBackground(uint16_t color)
{
    display->fillScreen(color);
}

void Display::drawIcon(uint16_t x, uint16_t y, const byte *bitmap, uint16_t width, uint16_t height, uint16_t color)
{
    display->drawBitmap(x, y, bitmap, width, height, color);
}

void Display::drawIcon(uint16_t y, const byte *bitmap, uint16_t width, uint16_t height, uint16_t color)
{
    uint16_t x = (display->width() - width) / 2;
    drawIcon(x, y, bitmap, width, height, color);
}

void Display::drawIcon(const byte *bitmap, uint16_t width, uint16_t height, uint16_t color)
{
    uint16_t x = (display->width() - width) / 2;
    uint16_t y = (display->height() - height) / 2;
    drawIcon(x, y, bitmap, width, height, color);
}

void Display::drawText(uint16_t x, uint16_t y, String message, uint8_t size, uint16_t color)
{
    display->setTextColor(color);
    display->setTextWrap(false);
    display->setTextSize(size);
    display->setCursor(x, y);
    display->print(message);
}

void Display::drawText(uint16_t y, String message, uint8_t size, uint16_t color)
{
    Bounds textBounds = getTextBounds(message, size);
    drawText(textBounds.centerOrigin.x, y, message, size, color);
}

void Display::drawText(String message, uint8_t size, uint16_t color)
{
    Bounds textBounds = getTextBounds(message, size);
    drawText(textBounds.centerOrigin.x, textBounds.centerOrigin.y, message, size, color);
}

void Display::iconWithText(uint16_t backgroundColor, uint16_t foregroundColor, const byte *bitmap, String message)
{
    uint16_t yCenter = display->height() / 2;

    Bounds textBounds = getTextBounds(message, 3);

    drawBackground(backgroundColor);
    drawIcon(30, bitmap, ICON_WIDTH, ICON_HEIGHT, foregroundColor);
    drawText(yCenter + textBounds.height, message, 3, foregroundColor);
}

Bounds Display::getTextBounds(String message, uint8_t size)
{
    int16_t x1, y1;
    uint16_t width, height;

    display->setTextSize(size);
    display->getTextBounds(message, 0, 0, &x1, &y1, &width, &height);

    int16_t x = (display->width() - width) / 2;
    int16_t y = (display->height() - height) / 2;

    return {width, height, {x, y}};
}
