#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "screen.h"

// Blank tile
#define TILE_BLANK 32

// Test application which displays all tiles
extern int loadFiles();

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

    screenConfig();

    // Clear the full tilemap for layer 1
    for (x = 0; x < 128; x++) {
        for (y = 0; y < 64; y++) {
            setTile(x,y,TILE_BLANK,0);
        }
    }

    // // Clear the full tilemap for layer 0
    // setBase(LAYER0_OFFSET);
    // for (x = 0; x < 64; x++) {
    //     for (y = 0; y < 32; y++) {
    //         setTile(x,y,TILE_BLANK,0);
    //     }
    // }
    
    setBase(LAYER1_OFFSET);

    for (x = 0; x < 16; x++) {
        for (y = 0; y < 8; y++) {
            setTile(x,y,(y*16)+x,0);
        }
    }

    setBase(LAYER0_OFFSET);
    for (x = 0; x < 16; x++) {
        for (y = 0; y < 8; y++) {
            setTile(x,y+10,(y*16)+x,0);
        }
    }

    do {
        if (kbhit()) {
            char c = cgetc();
        }
    } while (1);

    return result;
}