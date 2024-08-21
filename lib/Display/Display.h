#ifndef _DISPLAY_H_INCLUDED
#define _DISPLAY_H_INCLUDED

#include "DisplayDefines.h"

class Display
{
public:
    Display();

    uint16_t black = ST77XX_BLACK;
    uint16_t red = ST77XX_RED;

    /**
     * @brief Initialize the instance
     */
    bool begin();

    /**
     * @brief Destroy the instance
     */
    void end();

protected:
    Adafruit_ST7789 *display = nullptr;
};

extern Display display;

#endif //_DISPLAY_H_INCLUDED
