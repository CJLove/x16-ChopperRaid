#include "chopper.h"
#include "keys.h"
#include "screen.h"
#include "hud.h"
#include "bomb.h"
#include "turret.h"
#include "door.h"
#include "beams.h"
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



    // If not debugging chopper movement then enable the HUD
#if !defined(DEBUG_CHOPPER)
    displayStatic();

    // Enable other elements
    initDoors();
    initTurrets();
    initBeams();    
#endif    

    while (1) {
        waitvsync();

        switch (count) {
        case 0:
            updateChopper();
            updateBombs();
            break;
        case 1:
#if!defined(DEBUG_CHOPPER)        
            updateTurrets();
            updateBeams();
#endif            
            break;
        case 2:
            checkDoors();
            break;
        }
        count++;

        if (count == 3) count = 0;
    }

    return 0;
}