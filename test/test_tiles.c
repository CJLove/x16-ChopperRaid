#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "screen.h"

// Blank tile
#define TILE_BLANK 32

// Test application which displays all tiles
extern int loadFiles();
extern int loadTilemap();
extern int loadTilemap2();

int main()
{
    int result = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    screenConfig(SCREEN_CLEAR_L1 | SCREEN_CLEAR_L0);

    //loadTilemap();

    // Tiles on Layer1
    setBase(LAYER1_MAP_BASE);

    for (y = 0; y < 8; y++) {
        for (x = 0; x < 16; x++) {
            setTile(x,y,(y*16)+x,0);
        }
    }

    // Tiles on Layer0
    setBase(LAYER0_MAP_BASE);
    for (y = 0; y < 8; y++) {
        for (x = 0; x < 16; x++) {
            setTile(x+20,y,(y*16)+x,0);
        }
    }

    do {
        if (kbhit()) {
            char c = cgetc();
        }
    } while (1);

    return result;
}