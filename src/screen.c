#include <cx16.h>
#include "screen.h"

// Offset between rows in tilemap
#define OFFSET 256

static uint32_t screenBase = LAYER1_OFFSET;


int screenConfig()
{
    uint8_t x = 0;
    uint8_t y = 0;

    VERA.control = 0;

    VERA.layer0.config = 0;

    // VERA.layer0.config = 0xa2;      // 128x64 map, color depth 2
    // VERA.layer0.mapbase = 0x40;     // Map base at 0x08000
    // VERA.layer0.tilebase = 0x80;    // Tile base at 0x10000, 8x8 tiles
    // VERA.layer0.hscroll = 0;
    // VERA.layer0.vscroll = 0;

    VERA.layer1.config = 0xa2;      // 128x64 map, color depth 2
    VERA.layer1.mapbase = 0x0;      // Map base at 0x00000
    VERA.layer1.tilebase = 0x80;    // Tile base at 0x10000, 8x8 tiles

    VERA.display.video |= 0x20;     // Layer 1 + layer 0 enable


    videomode(VIDEOMODE_40x30);
    // Clear out layer0 tilemap
    setBase(LAYER0_OFFSET);
    for (x = 0; x < 128; x++) {
        for (y = 0; y < 30; y++) {
            setTile(x,y,32,0);
        }
    }
    setBase(LAYER1_OFFSET);
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
}

void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset)
{
    uint32_t addr = screenBase | ((uint32_t)VERA_INC_1 << 16) + y*OFFSET + x*2;
    vpoke(tile, addr);
    VERA.data0 = (paletteOffset << 4);
}

