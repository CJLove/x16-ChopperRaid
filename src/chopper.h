#include <cx16.h>
#include <stdint.h>



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
    uint8_t tx;
    uint8_t ty;
    uint8_t partialX;
    uint8_t partialY;
    uint8_t landed;
    uint8_t direction;
    uint8_t sequence;
    uint8_t idx;
    uint8_t ticks;
    uint8_t tiltCount;
};

extern struct chopper_t chopper;

// Chopper sequences
enum chopper_seq_e { CHOPPER_FULL_LEFT, CHOPPER_LEFT, CHOPPER_CENTER, CHOPPER_RIGHT, CHOPPER_FULL_RIGHT };


extern void initChopper(uint16_t x, uint16_t y);

extern void updateChopper(void);