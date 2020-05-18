#include <cx16.h>
#include <joystick.h>
#include "keys.h"

keyFcn inputHandler = keyHandler;

int8_t keyHandler(void)
{
    int8_t act = KEY_NONE;
    uint8_t joy = joy_read(0);
    if (joy & JOY_UP_MASK) {
        act |= KEY_UP;
    }
    if (joy & JOY_RIGHT_MASK) {
        act |= KEY_RIGHT;
    }
    if (joy & JOY_LEFT_MASK) {
        act |= KEY_LEFT;
    }
    if (joy & JOY_DOWN_MASK) {
        act |= KEY_DOWN;
    } 
    return act;
}