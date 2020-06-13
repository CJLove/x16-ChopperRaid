#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <joystick.h>
#include <unistd.h>
#include "chopper.h"
#include "screen.h"
#include "keys.h"
#include "turret.h"
#include "load.h"

// Test application which displays all tiles
extern int loadFiles();
extern int loadTilemap();

void debugScroll()
{
    char buf[40];
    int x = 0;
    int len = sprintf(buf,"%c: %3u %c: %3u",24,VERA.layer0.hscroll,25,VERA.layer0.vscroll);

    setBase(LAYER1_MAP_BASE);
    for (; x < len; x++) {
        setTile(x,0,buf[x],0);
    }
}

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
    result = initTurrets();


//    vera_sprites_enable(1);


    do {
        waitvsync();
#if 0
        key = inputHandler();
        if (key & KEY_RIGHT) {
            if (chopper.hscroll <= 700) {
                chopper.hscroll += 4;
                VERA.layer0.hscroll = chopper.hscroll;
            }
        } else if (key & KEY_LEFT) {
            if (chopper.hscroll >= 4) {
                chopper.hscroll -= 4;
                VERA.layer0.hscroll = chopper.hscroll;
            }
        }
        if (key & KEY_UP) {
            if (chopper.vscroll >= 4) {
                chopper.vscroll -= 4;
                VERA.layer0.vscroll = chopper.vscroll;
            }
        } else if (key & KEY_DOWN) {
            if (chopper.vscroll <= 312) {
                chopper.vscroll += 4;
                VERA.layer0.vscroll = chopper.vscroll;
            }
        }
#endif
        switch (count) {
        case 0:
            updateTurrets();
            break;
        case 1:
            updateChopper();
            break;
        case 2:
            break;
        }
        count++;
        if (count == 3) count = 0;
        debugScroll();
    } while (1);

    return result;
}