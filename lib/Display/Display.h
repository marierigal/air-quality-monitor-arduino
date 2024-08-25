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

    static const uint16_t BLACK = ST77XX_BLACK;
    static const uint16_t WHITE = ST77XX_WHITE;
    static const uint16_t RED = ST77XX_RED;
    static const uint16_t ORANGE = ST77XX_ORANGE;

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
     * @brief Convert RGB to 16-bit color
     * @param r The red component
     * @param g The green component
     * @param b The blue component
     * @return The 16-bit color
     */
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Display an error screen
     * @param message The message to display
     */
    void error(String message);

    /**
     * @brief Display a warning screen
     * @param message The message to display
     */
    void warning(String message);

    /**
     * @brief Set the background color
     */
    void drawBackground(uint16_t color = Display::BLACK);

    /**
     * @brief Draw an icon
     * @param x The x coordinate
     * @param y The y coordinate
     * @param bitmap The icon to display, 1-bit per pixel horizontal
     * @param width The width of the icon
     * @param height The height of the icon
     * @param color The color of the icon
     */
    void drawIcon(uint16_t x, uint16_t y, const byte *bitmap, uint16_t width, uint16_t height, uint16_t color = Display::WHITE);

    /**
     * @brief Draw an icon centered horizontally on the screen
     * @param y The y coordinate
     * @param bitmap The icon to display, 1-bit per pixel horizontal
     * @param width The width of the icon
     * @param height The height of the icon
     * @param color The color of the icon
     */
    void drawIcon(uint16_t y, const byte *bitmap, uint16_t width, uint16_t height, uint16_t color = Display::WHITE);

    /**
     * @brief Draw an icon centered on the screen
     * @param bitmap The icon to display, 1-bit per pixel horizontal
     * @param width The width of the icon
     * @param height The height of the icon
     * @param color The color of the icon
     */
    void drawIcon(const byte *bitmap, uint16_t width, uint16_t height, uint16_t color = Display::WHITE);

    /**
     * @brief Draw a text
     * @param x The x coordinate
     * @param y The y coordinate
     * @param message The message to display
     * @param size The size of the text
     * @param color The color of the text
     * @param wrap True to wrap the text
     */
    void drawText(uint16_t x, uint16_t y, String message, uint8_t size, uint16_t color = Display::WHITE, uint16_t background = Display::BLACK);

    /**
     * @brief Draw a text centered horizontally on the screen
     * @param y The y coordinate
     * @param message The message to display
     * @param size The size of the text
     * @param color The color of the text
     */
    void drawText(uint16_t y, String message, uint8_t size, uint16_t color = Display::WHITE, uint16_t background = Display::BLACK);

    /**
     * @brief Draw a text centered on the screen
     * @param message The message to display
     * @param size The size of the text
     * @param color The color of the text
     */
    void drawText(String message, uint8_t size, uint16_t color = Display::WHITE, uint16_t background = Display::BLACK);

    /**
     * @brief Get the origin coordinates of the text to center it
     * @param message The message to display
     *
     * @return The bounds of the text
     */
    Bounds getTextBounds(String message, uint8_t size);

protected:
    Adafruit_ST7789 *display = nullptr;

    /**
     * @brief Display an icon with a message
     * @param backgroundColor The background color
     * @param foregroundColor The foreground color for the text and icon
     * @param bitmap The icon to display, 1-bit per pixel horizontal
     * @param message The message to display
     */
    void iconWithText(uint16_t backgroundColor, uint16_t foregroundColor, const byte *bitmap, String message);
};

#endif //_DISPLAY_H_INCLUDED
