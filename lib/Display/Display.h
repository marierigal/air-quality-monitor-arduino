#ifndef _DISPLAY_H_INCLUDED
#define _DISPLAY_H_INCLUDED

// Display libraries
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS 2
#define TFT_DC 1
#define TFT_RST -1

#include "bitmapError.h"
#include "bitmapWarning.h"
#define ICON_WIDTH 64
#define ICON_HEIGHT 64

struct Coordinate
{
    int16_t x;
    int16_t y;
};

struct Bounds
{
    uint16_t width;
    uint16_t height;
    Coordinate centerOrigin;
};

class Display
{
public:
    Display();
    ~Display();

    uint16_t black = ST77XX_BLACK;
    uint16_t white = ST77XX_WHITE;
    uint16_t red = ST77XX_RED;
    uint16_t orange = ST77XX_ORANGE;

    /**
     * @brief Initialize the instance
     */
    bool begin();

    /**
     * @brief Destroy the instance
     */
    void end();

    /**
     * @brief Set the color to black for all pixels
     */
    void clear();

    /**
     * @brief Display an error screen
     * @param message The message to display
     */
    void error(const char *message);

    /**
     * @brief Display a warning screen
     * @param message The message to display
     */
    void warning(const char *message);

protected:
    Adafruit_ST7789 *display = nullptr;

    /**
     * @brief Display an icon with a message
     * @param backgroundColor The background color
     * @param foregroundColor The foreground color for the text and icon
     * @param bitmap The icon to display, 1-bit per pixel horizontal
     * @param message The message to display
     */
    void iconWithText(uint16_t backgroundColor, uint16_t foregroundColor, const byte *bitmap, const char *message);

    /**
     * @brief Get the origin coordinates of the text to center it
     * @param message The message to display
     *
     * @return The bounds of the text
     */
    Bounds getTextBounds(const char *message);
};

extern Display display;

#endif //_DISPLAY_H_INCLUDED
