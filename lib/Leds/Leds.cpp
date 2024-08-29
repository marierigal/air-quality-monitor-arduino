#include "Leds.h"

Leds::Leds()
{
}

Leds::~Leds()
{
    end();
}

bool Leds::begin()
{
    if (leds == nullptr)
    {
        leds = new Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
    }

    leds->begin();

    leds->setBrightness(BRIGHTNESS);
    leds->clear();
    leds->show();

    return true;
}

void Leds::end()
{
    clear();

    delete leds;
    leds = nullptr;
}

void Leds::all(uint32_t color)
{
    leds->fill(color, 0, NUMPIXELS);
    leds->show();
}

void Leds::set(uint8_t index, uint32_t color)
{
    leds->setPixelColor(index, color);
    leds->show();
}

void Leds::clear()
{
    all(black);
}

void Leds::setBrightness(uint8_t brightness)
{
    leds->setBrightness(brightness);
}

void Leds::error()
{
    all(red);
}

void Leds::warning()
{
    all(orange);
}

void Leds::success()
{
    all(green);
}
