#ifndef _QTOUCH_H_INCLUDED_
#define _QTOUCH_H_INCLUDED_

#include "Arduino_MCHPTouch.h"

enum touch_pads
{
    Y0 = 0,
    Y1 = 8,
    Y2 = 9,
    Y3 = 2,
    Y4 = 3,
};

typedef enum
{
    TOUCH0 = 0,
    TOUCH1,
    TOUCH2,
    TOUCH3,
    TOUCH4
} touchButton;

class Qtouch
{
public:
    Qtouch();
    ~Qtouch();

    /**
     * @brief Initialize the instance
     */
    bool begin(uint touchSensitivity = 100);

    /**
     * @brief Destroy the instance
     */
    void end();

    /**
     * @brief Poll the sensor for new measure
     */
    bool update();

    /**
     * @brief Checks if the button is being touched
     * @param padIndex the index of the pad to check
     * @return true if new data are available
     */
    bool getTouch(touchButton padIndex);

    /**
     * @brief Checks if the button has been touched
     * @param padIndex the index of the pad to check
     * @return true if new data are available
     */
    bool onTouchDown(touchButton padIndex);

    /**
     * @brief Checks if the button has been released
     * @param padIndex the index of the pad to check
     * @return true if new data are available
     */
    bool onTouchUp(touchButton padIndex);

    /**
     * @brief Checks if the button as changed state
     * @param padIndex the index of the pad to check
     * @return true if new data are available
     */
    bool onTouchChange(touchButton padIndex);

private:
    bool _available = false;
    bool _touchesPrev[5] = {0, 0, 0, 0, 0}; // To know last touches
};

#endif // _QTOUCH_H_INCLUDED_
