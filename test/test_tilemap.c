#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <joystick.h>
#include <unistd.h>
#include "screen.h"
#include "keys.h"

// Test application which displays all tiles
extern int loadFiles();
extern int loadTilemap();

int main()
{
    int result = 0;
    uint16_t hscroll = 0;
    uint16_t vscroll = 0;
    uint8_t key = 0;
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    // Install CX16 joystick driver
    joy_install(cx16_std_joy);

    // Setup input handler for joystick/keyboard
    inputHandler = keyHandler;

    screenConfig(SCREEN_CLEAR_L0);


    
    loadTilemap();
    do {
        waitvsync();

        key = inputHandler();
        if (key & KEY_RIGHT) {
            hscroll += 4;
            VERA.layer1.hscroll = hscroll;
        } else if (key & KEY_LEFT) {
            hscroll -= 4;
            VERA.layer1.hscroll = hscroll;
        }
        if (key & KEY_UP) {
            vscroll -= 4;
            VERA.layer1.vscroll = vscroll;
        } else if (key & KEY_DOWN) {
            vscroll += 4;
            VERA.layer1.vscroll = vscroll;
        }
    } while (1);

    return result;
}