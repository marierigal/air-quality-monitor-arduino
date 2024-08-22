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

    leds->setBrightness(BRIGHNESS);
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
}

void Leds::clear()
{
    all(black);
    leds->show();
}

void Leds::error()
{
    all(red);
    leds->show();
}

void Leds::warning()
{
    all(orange);
    leds->show();
}

void Leds::success()
{
    all(green);
    leds->show();
}
