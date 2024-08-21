#ifndef _LEDS_H_INCLUDED
#define _LEDS_H_INCLUDED

#include "LedsDefines.h"

class Leds
{
public:
    Leds();

    uint32_t black = 0x000000;
    uint32_t red = 0xFF0000;
    uint32_t orange = 0xFFA500;
    uint32_t green = 0x00FF00;

    /**
     * @brief Initialize the instance
     */
    bool begin();

    /**
     * @brief Destroy the instance
     */
    void end();

    /**
     * @brief Set the color to all LEDs
     */
    void all(uint32_t color);

    /**
     * @brief Turn off the LEDs
     */
    void clear();

    /**
     * @brief Turn on the LEDs with a red color
     */
    void error();

    /**
     * @brief Turn on the LEDs with an orange color
     */
    void warning();

    /**
     * @brief Turn on the LEDs with a green color
     */
    void success();

protected:
    Adafruit_DotStar *leds = nullptr;
};

extern Leds leds;

#endif //_LEDS_H_INCLUDED
