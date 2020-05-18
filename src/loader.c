#include <stdio.h>
#include <cbm.h>
#include <cx16.h>
#include "vload.h"
#include "load.h"

int loadTilemap()
{
    int result = 1;

    // Load tilemap
    result = vload_host("tilemap.bin",0x00000);
    if (result)
        return result;
    else {
        return 0;
    }
}

int loadFiles()
{
    int result = 1;

    // Load palette
    result = vload_host("palette.bin",0x1fa00);
    if (result) 
        printf("  loaded palette\n");
    else {
        printf("  failed to load palette\n");
        return 0;
    }

    // Load tiles
    result = vload_host("tiles.bin",0x10000);
    if (result)
        printf("  loaded tiles\n");
    else {
        printf("  failed to load tiles\n");
        return 0;
    }

    // Load sprites
    result = vload_host("sprites.bin",0x1c000);
    if (result)
        printf("  loaded sprites\n");
    else {
        printf("  failed to load sprites\n");
        return 0;
    }



    return result;
}