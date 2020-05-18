#include <cx16.h>
#include "screen.h"

// Offset between rows in tilemap
#define OFFSET 256



int screenConfig()
{
    uint8_t x = 0;
    uint8_t y = 0;

    VERA.control = 0;
    VERA.display.video |= 0x20;      // Layer 1 enable

    // VERA.display.hscale = 128;       // HSCALE=1
    // VERA.display.vscale = 128;       // VSCALE=1
    VERA.layer0.config = 0x0;       // Disable Layer 0

    VERA.layer1.config = 0xa2;      // 128x64 map, color depth 2
    VERA.layer1.mapbase = 0x0;      // Map base at 0x00000
    VERA.layer1.tilebase = 0x80;    // Tile base at 0x04000, 8x8 tiles

    videomode(VIDEOMODE_40x30);

    return 1;

}

int screenReset()
{
    videomode(VIDEOMODE_80x60);

    return 1;
}

void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset)
{
    unsigned long addr = ((uint32_t)VERA_INC_1 << 16) | y*OFFSET + x*2;
    vpoke(tile, addr);
    VERA.data0 = (paletteOffset << 4);
}

