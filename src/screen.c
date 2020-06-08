#include <cx16.h>
#include "screen.h"
#include "vera.h"
#include <stdio.h>

// Buffer holding the current tiles under the chopper
uint8_t tiles[4][7];

// Blank tile
#define TILE_BLANK 32
// Tilemap dimensions
#define MAP_WIDTH 128
#define MAP_HEIGHT 64
// Offset between rows in tilemap
#define MAP_OFFSET 256

// Representation of tilemap metadata as stored in RAM bank 1
struct TilemapMetadata_t {
    uint8_t tiles[MAP_HEIGHT][MAP_WIDTH];
};

static struct TilemapMetadata_t *meta = (struct TilemapMetadata_t*)0xa000;

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
    uint32_t addr = screenBase | ((uint32_t)VERA_INC_1 << 16) + y*MAP_OFFSET + x*2;
    vpoke(tile, addr);
    VERA.data0 = (paletteOffset << 4);
}

void setMetaTile(uint8_t x, uint8_t y, uint8_t tile)
{
    meta->tiles[y][x] = tile;
}

void getTiles(uint8_t x, uint8_t y)
{
    // Access tile metadata from RAM bank 1
    int i = 0;
    uint8_t *layer = (uint8_t*)0xa000;
    uint16_t idx = y*MAP_WIDTH + x;
    // Switch to bank 1
    VIA1.pra = 1;
    for (i = 0; i < 7; i++) {
        if (y <= 63) {
            tiles[0][i] = meta->tiles[y][x+i];
        } else {
            tiles[0][i] = tiles[1][i] = tiles[2][i] = tiles[3][i] = TILE_BLANK;
        }
        if (y <= 62) {
            tiles[1][i] = meta->tiles[y+1][x+i];
        } else {
            tiles[1][i] = tiles[2][i] = tiles[3][i] = TILE_BLANK;
        }
        if (y <= 61) {
            tiles[2][i] = meta->tiles[y+2][x+i];
        } else {
            tiles[2][i] = tiles[3][i] = TILE_BLANK;
        }
        if (y <= 60) {
            tiles[3][i] = meta->tiles[y+3][x+i];
        } else {
            tiles[3][i] = TILE_BLANK;
        }
    }   
}
