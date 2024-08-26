#include "Qtouch.h"

Qtouch::Qtouch()
{
}

Qtouch::~Qtouch()
{
    end();
}

bool Qtouch::begin(uint touchSensitivity)
{
    TOUCH.setTouchPad(Y0, Y1, Y2, Y3, Y4);
    TOUCH.setSensorsSensitivity(touchSensitivity);
    return TOUCH.begin();
}

void Qtouch::end()
{
}

bool Qtouch::update()
{
    TOUCH.poll();
    _available = TOUCH.available();
    return _available;
}

bool Qtouch::getTouch(touchButton padIndex)
{
    if (!_available)
        return false;

    return TOUCH.read(padIndex);
}

bool Qtouch::onTouchDown(touchButton padIndex)
{
    if (!_available)
        return false;

    if (getTouch(padIndex) && _touchesPrev[padIndex] == false)
    {
        _touchesPrev[padIndex] = true;
        return true;
    }

    _touchesPrev[padIndex] = getTouch(padIndex);
    return false;
}

bool Qtouch::onTouchUp(touchButton padIndex)
{
    if (!_available)
        return false;

    if (!getTouch(padIndex) && _touchesPrev[padIndex] == true)
    {
        _touchesPrev[padIndex] = false;
        return true;
    }

    _touchesPrev[padIndex] = getTouch(padIndex);
    return false;
}

bool Qtouch::onTouchChange(touchButton padIndex)
{
    if (!_available)
        return false;

    if (_touchesPrev[padIndex] != getTouch(padIndex))
    {
        _touchesPrev[padIndex] = getTouch(padIndex);
        return true;
    }

    return false;
}
