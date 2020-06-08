#include <stdint.h>

#define LAYER0_MAP_BASE     0x0000
#define LAYER1_MAP_BASE     0x4000
#define TILESET_BASE        0x10000
#define SPRITES_BASE        0x1c000

extern uint8_t tiles[4][7];

// Options for screenConfig()
enum ScreenOpts_e {
    SCREEN_OPT_NONE = 0, 
    SCREEN_CLEAR_L0 = 0x1,
    SCREEN_CLEAR_L1 = 0x2
} ;

extern int screenConfig(uint8_t opts);

extern int screenReset(void);

extern void clearMap(uint32_t base);

extern void setBase(uint32_t base);

extern void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset);

extern void setMetaTile(uint8_t x, uint8_t y, uint8_t tile);

extern void getTiles(uint8_t x, uint8_t y);

