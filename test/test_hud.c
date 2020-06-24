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
    uint8_t zone = 1;
    uint32_t score = 10000;
    uint8_t lives = 0;
    uint8_t keys = 0;
    uint8_t jammers = 0;
    uint16_t fuel = 33;
    uint16_t ammo = 33;
    uint8_t unit = 4;
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    screenConfig(SCREEN_CLEAR_L1);
    // Install CX16 joystick driver
    joy_install(cx16_std_joy);

    // Setup input handler for joystick/keyboard
    inputHandler = keyHandler;

    loadTilemap();
    initChopper(50, 50);    
    displayStatic();

    do {

        if (kbhit()) {
            uint8_t c = cgetc();

            switch (c) {
                case 'z':
                    zone++;
                    displayZone(zone);
                    break;
                case 's':
                    score += 100;
                    displayScore(score);
                    break;
                case 'l':
                    lives++;
                    displayLives(lives);
                    break;
                case 'k':
                    keys++;
                    displayKeys(keys);
                    break;
                case 'j':
                    jammers++;
                    displayJammers(jammers);
                    break;
                case 'f':
                    if (fuel) {
                        fuel--;
                        displayFuel(fuel);
                    }
                    break;
                case 'F':
                    fuel = 33;
                    displayFuel(fuel);
                    break;
                case 'a':
                    if (ammo) {
                        ammo--;
                        displayAmmo(ammo);
                    }
                    break;
                case 'A':
                    ammo = 33;
                    displayAmmo(ammo);
                    break;
                case ' ':
                    displayToken(unit);
                    unit++;
                    break;
                default:
                    break;
            }
        }
    } while (1);

    return result;
}