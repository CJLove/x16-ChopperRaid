#include <stdint.h>

typedef int8_t (*keyFcn)(void);

extern keyFcn inputHandler;

extern int8_t keyHandler(void);

enum {
    KEY_NONE = 0,
    KEY_LEFT = 0x1,
    KEY_RIGHT = 0x2,
    KEY_UP = 0x4,
    KEY_DOWN = 0x8,
    KEY_FIRE = 0x10,
    KEY_A = 0x20,
    KEY_B = 0x40
};