#include "chopper.h"
#include "screen.h"
#include "vera.h"
#include <conio.h>
#include <cx16.h>
#include <stdio.h>
#include <unistd.h>

// Test application which displays all sprites
extern int loadFiles();

int main()
{
    int result = 0;
    uint8_t tile = 0;
    uint16_t x = 16;
    uint16_t y = 16;
    uint32_t sprite = ((uint32_t)VERA_INC_1 << 16) | SPRITE_ATTR0;

    printf("loading resources...\n");

    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    }
    else {
        printf("failed to load all resources\n");
        return result;
    }

    screenConfig(SCREEN_CLEAR_L0 | SCREEN_CLEAR_L1);

    x = 16;
    y = 16;

    // Chopper sprite images
    vpoke(SPRITE_ADDR_L(CHOPPER_CENTER_1), sprite);   // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_CENTER_1);     // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_CENTER_2), sprite);   // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_CENTER_2);     // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_CENTER_3), sprite);   // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_CENTER_3);     // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x = 16;
    y += 40;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_RIGHT_1), sprite);    // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_RIGHT_1);      // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_RIGHT_2), sprite);    // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_RIGHT_2);      // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_RIGHT_3), sprite);    // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_RIGHT_3);      // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x = 16;
    y += 40;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_TILT_RIGHT_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_TILT_RIGHT_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                          // Attr2
    VERA.data0 = SPRITE_X_H(x);                          // Attr3
    VERA.data0 = SPRITE_Y_L(y);                          // Attr4
    VERA.data0 = SPRITE_Y_H(y);                          // Attr5
    VERA.data0 = SPRITE_LAYER1;                          // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;     // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_TILT_RIGHT_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_TILT_RIGHT_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                          // Attr2
    VERA.data0 = SPRITE_X_H(x);                          // Attr3
    VERA.data0 = SPRITE_Y_L(y);                          // Attr4
    VERA.data0 = SPRITE_Y_H(y);                          // Attr5
    VERA.data0 = SPRITE_LAYER1;                          // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;     // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_TILT_RIGHT_3), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_TILT_RIGHT_3);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                          // Attr2
    VERA.data0 = SPRITE_X_H(x);                          // Attr3
    VERA.data0 = SPRITE_Y_L(y);                          // Attr4
    VERA.data0 = SPRITE_Y_H(y);                          // Attr5
    VERA.data0 = SPRITE_LAYER1;                          // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;     // Attr7

    // Enable sprites
    vera_sprites_enable(1);

    while (1) {
        waitvsync();
    }

    return result;
}