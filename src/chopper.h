#include <cx16.h>
#include <stdint.h>

// Base sprite attribute addresses for sprite 0
#define SPRITE_ATTR0    0x11fc00
#define SPRITE_ATTR1    0x1fc01
#define SPRITE_ATTR2    0x1fc02
#define SPRITE_ATTR3    0x1fc03
#define SPRITE_ATTR4    0x1fc04
#define SPRITE_ATTR5    0x1fc05
#define SPRITE_ATTR6    0x1fc06
#define SPRITE_ATTR7    0x1fc07

// Chopper sprite image addresses
#define CHOPPER_CENTER_1    0x1c000
#define CHOPPER_CENTER_2    0x1c400
#define CHOPPER_CENTER_3    0x1c800
#define CHOPPER_RIGHT_1     0x1cc00
#define CHOPPER_RIGHT_2     0x1d000
#define CHOPPER_RIGHT_3     0x1d400
#define CHOPPER_TILT_RIGHT_1    0x1d800
#define CHOPPER_TILT_RIGHT_2    0x1dc00
#define CHOPPER_TILT_RIGHT_3    0x1e000

struct chopper_t {
    uint16_t hscroll;
    uint16_t vscroll;
    uint16_t x;
    uint16_t y;
    uint8_t direction;
    uint8_t sequence;
    uint8_t idx;
    uint8_t ticks;
    uint8_t tiltCount;

};

extern struct chopper_t chopper;

extern void initChopper(uint16_t x, uint16_t y);

extern void updateChopper();