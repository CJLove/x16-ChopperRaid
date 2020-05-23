#include <cx16.h>
#include "screen.h"
#include "vera.h"
#include <stdio.h>


// Blank tile
#define TILE_BLANK 32
// Tilemap dimensions
#define MAP_WIDTH 128
#define MAP_HEIGHT 64
// Offset between rows in tilemap
#define MAP_OFFSET 256

// Base address for setTile()
static uint32_t screenBase = LAYER1_MAP_BASE;

int screenConfig(uint8_t opts)
{
    uint8_t x = 0;
    uint8_t y = 0;

    VERA.control = 0;
    videomode(VIDEOMODE_40x30);

    // Layer 0 - scrolling tilemap
    VERA.layer0.config = MAP_WIDTH_128 | MAP_HEIGHT_64 | LAYER_CONFIG_4BPP;
    VERA.layer0.mapbase = MAP_BASE_ADDR(LAYER0_MAP_BASE); 
    VERA.layer0.tilebase = TILE_BASE_ADDR(TILESET_BASE) | TILE_WIDTH_8 | TILE_HEIGHT_8;
    VERA.layer0.hscroll = 0;
    VERA.layer0.vscroll = 0;

    // Layer 1 - overlay
    VERA.layer1.config = MAP_WIDTH_128 | MAP_HEIGHT_64 | LAYER_CONFIG_4BPP;
    VERA.layer1.mapbase = MAP_BASE_ADDR(LAYER1_MAP_BASE);
    VERA.layer1.tilebase = TILE_BASE_ADDR(TILESET_BASE) | TILE_WIDTH_8 | TILE_HEIGHT_8;
    VERA.layer1.hscroll = 0;
    VERA.layer1.vscroll = 0;


    if (opts & SCREEN_CLEAR_L0)
    {
        // Clear the full tilemap for layer 0
        clearMap(LAYER0_MAP_BASE);
    }

    if (opts & SCREEN_CLEAR_L1) {
        // Clear the full tilemap for layer 1
        clearMap(LAYER1_MAP_BASE);
    }

    VERA.display.video |=  LAYER0_ENABLED |  LAYER1_ENABLED;

    return 1;

}

int screenReset()
{
    videomode(VIDEOMODE_80x60);

    return 1;
}

void setBase(uint32_t base)
{
    screenBase = base;
    //printf("base=0x%lx\n",screenBase);
}

void clearMap(uint32_t base)
{
    uint32_t addr = base | ((uint32_t)VERA_INC_1 << 16);
    uint16_t size = (MAP_HEIGHT * MAP_WIDTH)-1;
    uint16_t i = 0;

    // Clear the first tile and palette offset
    vpoke(TILE_BLANK,addr);
    VERA.data0 = 0;

    for (i = 0; i < size; i++) {
        VERA.data0 = TILE_BLANK;
        VERA.data0 = 0;
    }
}

void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset)
{
#if 1
    uint32_t addr = screenBase | ((uint32_t)VERA_INC_1 << 16) + y*MAP_OFFSET + x*2;
    vpoke(tile, addr);
    VERA.data0 = (paletteOffset << 4);
#else
    uint32_t addr = screenBase + y*OFFSET + x*2;
    vpoke(tile, addr);
    vpoke((paletteOffset << 4),addr+1);
#endif
}

