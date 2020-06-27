#include <stdint.h>

#define BOMB_IMAGE  0x1f280

struct bomb_t {
    // Bomb location
    uint16_t x;
    uint16_t y;

    // Offset for sprite attributes
    uint16_t spriteOffset;

    // Bomb is active/in flight
    uint8_t active;
};

// Init bomb configuration data
void initBombs(void);

// Entry point from main loop
void updateBombs(void);

// Drop a bomb
void dropBomb(void);

