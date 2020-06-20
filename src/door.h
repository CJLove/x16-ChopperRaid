#include <stdint.h>



struct door_t {
    // Door state: closed, opening, opened
    uint8_t state;
    uint8_t delay;

    // Tile coordinates of top door tile
    uint8_t x;
    uint8_t y;

    uint8_t height;

};

void initDoors(void);

void checkDoors(void);