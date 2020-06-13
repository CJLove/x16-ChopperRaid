
#include <stdint.h>

// Turret sprite image addresses
#define TURRET_UP_LEFT      0x1f000
#define TURRET_UP_UP        0x1f040
#define TURRET_UP_RIGHT     0x1f080
#define TURRET_UP_EXPLODE   0x1f0c0

#define TURRET_LEFT_LEFT    0x1f100
#define TURRET_LEFT_UP      0x1f140
#define TURRET_LEFT_RIGHT   0x1f180
#define TURRET_LEFT_EXPLODE 0x1f1c0



enum turretOrientation_t {
    TURRET_UP,      // Facing up
    TURRET_DOWN,    // Facing down
    TURRET_LEFT,    // Facing left
    TURRET_RIGHT    // Facing right
};

enum turretAngle_t {
    ANGLE_LEFT,
    ANGLE_UP,
    ANGLE_RIGHT     // Horizontal or vertical flip of ANGLE_LEFT image
};

struct turret_t {
    // Turret pixel position
    uint16_t x;
    uint16_t y;  
    // Offset for sprite attributes
    uint16_t spriteOffset;

    // Direction the turret is facing
    uint8_t orientation;
    // Turret state (active/disabled/explodingn)
    uint8_t active;
    // Turret angle
    uint8_t angle;
    // Turret movement direction/amount
    int8_t direction;
    // Max move increments in supported direction
    uint8_t maxMove;
    // Current move increment
    uint8_t curMove;
    // Turret damage count (number of hits to destroy)
    uint8_t damageCount;

    // bits 0 & 1 for sprite attribute 6
    uint8_t flip;

    // Tile dimensions of turrent limits
    uint8_t limit1_x;
    uint8_t limit1_y;
    uint8_t limit2_x;
    uint8_t limit2_y;

};

// Entry point to initialize turrets for a particular level
extern int initTurrets(void);

// Entry point from main loop to move/update turrets based on the 
// visible portion of the tilemap
extern void updateTurrets(void);

