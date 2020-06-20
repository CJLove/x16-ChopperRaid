#include "chopper.h"
#include "keys.h"
#include "screen.h"
#include <conio.h>
#include <cx16.h>
#include <joystick.h>
#include <stdint.h>
#include <stdio.h>

extern int loadFiles();
extern int loadTilemap();

int main()
{
    int i = 0;
    int result = 0;
    uint16_t count = 0;
    result = loadFiles();

    // Install CX16 joystick driver
    joy_install(cx16_std_joy);

    // Setup input handler for joystick/keyboard
    inputHandler = keyHandler;

    screenConfig(SCREEN_CLEAR_L1);

    loadTilemap();

    initChopper(50, 50);

    while (1) {
        waitvsync();

        switch (count) {
        case 0:
            updateChopper();
            break;
        case 1:
            break;
        case 2:
            break;
        }
        count++;

        if (count == 3) count = 0;
    }

    return 0;
}