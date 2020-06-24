#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <joystick.h>
#include <unistd.h>
#include "chopper.h"
#include "screen.h"
#include "keys.h"
#include "door.h"
#include "load.h"
#include "hud.h"

// Test application which displays all tiles
extern int loadFiles();
extern int loadTilemap();

int main()
{
    int result = 0;
    uint8_t key = 0;
    uint8_t count = 0;

    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    // Global state for hscroll and vscroll
    chopper.hscroll = 0;
    chopper.vscroll = 0;

    // Install CX16 joystick driver
    joy_install(cx16_std_joy);

    // Setup input handler for joystick/keyboard
    inputHandler = keyHandler;      

    screenConfig(SCREEN_CLEAR_L1);

    loadTilemap();
    initChopper(50, 50);    
    initDoors();

#if !defined(DEBUG_DOORS)
    displayStatic();
#endif    

    vera_sprites_enable(1);

    do {
        waitvsync();

        switch (count) {
        case 0:
            
            break;
        case 1:
            updateChopper();
            break;
        case 2:
            checkDoors();
            break;
        }
        count++;
        if (count == 3) count = 0;
        //debugScroll();
    } while (1);
    return result;
}