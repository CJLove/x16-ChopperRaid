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

    screenConfig(SCREEN_CLEAR_L1);


    
    loadTilemap();
    do {
        waitvsync();

        key = inputHandler();
        if (key & KEY_RIGHT) {
            if (hscroll <= 700) {
                hscroll += 4;
                VERA.layer0.hscroll = hscroll;
            }
        } else if (key & KEY_LEFT) {
            if (hscroll >= 4) {
                hscroll -= 4;
                VERA.layer0.hscroll = hscroll;
            }
        }
        if (key & KEY_UP) {
            if (vscroll >= 4) {
                vscroll -= 4;
                VERA.layer0.vscroll = vscroll;
            }
        } else if (key & KEY_DOWN) {
            if (vscroll <= 312) {
                vscroll += 4;
                VERA.layer0.vscroll = vscroll;
            }
        }
        debugScroll();
    } while (1);

    return result;
}