#ifndef _DISPLAY_H_INCLUDED
#define _DISPLAY_H_INCLUDED

// Display libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Org_01.h>

#ifndef WHITE
#define WHITE 1
#endif

#ifndef BLACK
#define BLACK 0
#endif

#define SCREEN_CS 2
#define SCREEN_DC 1
#define SCREEN_RST -1
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

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
    const uint16_t width = SCREEN_WIDTH;
    const uint16_t height = SCREEN_HEIGHT;

    Display();
    ~Display();

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
     * @brief Show the display
     */
    void show();

    /**
     * @brief Draw an icon
     * @param x The x coordinate
     * @param y The y coordinate
     * @param bitmap The icon to display, 1-bit per pixel horizontal
     * @param width The width of the icon
     * @param height The height of the icon
     * @param color The color of the icon
     */
    void drawIcon(uint16_t x, uint16_t y, const byte *bitmap, uint16_t width, uint16_t height);

    /**
     * @brief Draw a text
     * @param x The x coordinate
     * @param y The y coordinate
     * @param message The message to display
     * @param size The size of the text
     * @param color The color of the text
     * @param wrap True to wrap the text
     */
    void drawText(uint16_t x, uint16_t y, String message, uint8_t size, bool wrap);

    /**
     * @brief Get the origin coordinates of the text to center it
     * @param message The message to display
     *
     * @return The bounds of the text
     */
    Bounds getTextBounds(String message, uint8_t size, bool wrap);

protected:
    Adafruit_SSD1306 *display = nullptr;
};

#endif //_DISPLAY_H_INCLUDED
