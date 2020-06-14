#include <stdio.h>
#include <cbm.h>
#include <cx16.h>
#include "vload.h"
#include "load.h"
#include "vera.h"
#include "screen.h"

int loadTilemap()
{
    int result = 1;

    // Load tilemap
    result = vload_host("tilemap.bin",LAYER0_MAP_BASE);
    if (result) {
        // Load tilemap metadata to bank 1
        result = load_bank_host("mapmeta.bin",1);
        if (result) {
            return 1;
        }
        return 0;
    }
    else {
        return 0;
    }
}

int loadFiles()
{
    int result = 1;

    // Load palette
    result = vload_host("palette.bin",VERA_PALETTE_BASE);
    if (result) 
        printf("  loaded palette\n");
    else {
        printf("  failed to load palette\n");
        return 0;
    }

    // Load tiles
    result = vload_host("tiles.bin",TILESET_BASE);
    if (result)
        printf("  loaded tiles\n");
    else {
        printf("  failed to load tiles\n");
        return 0;
    }

    // Load sprites
    result = vload_host("sprites.bin",SPRITES_BASE);
    if (result)
        printf("  loaded sprites\n");
    else {
        printf("  failed to load sprites\n");
        return 0;
    }
    result = vload_host("hturret.bin",SPRITES_BASE + 0x3000);
    if (result)
        printf("  loaded hturret sprites\n");
    else {
        printf("  failed to load hturret sprites\n");
        return 0;
    }
    result = vload_host("vturret.bin",SPRITES_BASE + 0x3100);
    if (result)
        printf("  loaded vturet sprites\n");
    else {
        printf("  failed to load vturret sprites\n");
        return 0;
    }
    result = vload_host("proj.bin",SPRITES_BASE + 0x3200);
    if (result) 
        printf("  loaded projectile sprites\n");
    else {
        printf("  failed to load projectile sprites\n");
        return 0;
    }

    // Load tile/sprite bitmap data to bank 2
    result = load_bank_host("meta.bin",2);
    if (result) 
        printf("  loaded tile/sprite metadata\n");
    else {
        printf("  failed to load tile/sprite metadata\n");
        return 0;
    }
    return result;
}