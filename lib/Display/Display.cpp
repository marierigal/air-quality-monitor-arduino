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
        display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RST);

        display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
        display->clearDisplay();
        display->display();

        // Set custom font
        display->setFont(&Org_01);
    }

    return true;
}

void Display::end()
{
    clear();

    // delete display;
    display = nullptr;
}

void Display::clear()
{
    display->clearDisplay();
}

void Display::show()
{
    display->display();
}

void Display::drawIcon(uint16_t x, uint16_t y, const byte *bitmap, uint16_t width, uint16_t height)
{
    display->drawBitmap(x, y, bitmap, width, height, WHITE);
}

void Display::drawText(uint16_t x, uint16_t y, String message, uint8_t size, bool wrap)
{
    display->setTextColor(WHITE);
    display->setTextWrap(wrap);
    display->setTextSize(size);
    display->setCursor(x, y);
    display->print(message);
}

Bounds Display::getTextBounds(String message, uint8_t size, bool wrap)
{
    int16_t x1, y1;
    uint16_t width, height;

    display->setTextWrap(wrap);
    display->setTextSize(size);
    display->getTextBounds(message, 0, height, &x1, &y1, &width, &height);

    return {width, height};
}
