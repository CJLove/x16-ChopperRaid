#include "beams.h"
#include "chopper.h"
#include "screen.h"
#include "turret.h"
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

    x = 16;
    y += 40;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_EXPLODE_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_EXPLODE_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_EXPLODE_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_EXPLODE_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    x += 70;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(CHOPPER_EXPLODE_3), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_EXPLODE_3);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;  // Attr7

    // Horizontal turret 8x16
    x = 16;
    y += 40;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_UP_LEFT), sprite);    // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_UP_LEFT);      // Attr1
    VERA.data0 = SPRITE_X_L(x);                      // Attr2
    VERA.data0 = SPRITE_X_H(x);                      // Attr3
    VERA.data0 = SPRITE_Y_L(y);                      // Attr4
    VERA.data0 = SPRITE_Y_H(y);                      // Attr5
    VERA.data0 = SPRITE_LAYER1 | 2;                  // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_16;  // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_UP_UP), sprite);      // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_UP_UP);        // Attr1
    VERA.data0 = SPRITE_X_L(x);                      // Attr2
    VERA.data0 = SPRITE_X_H(x);                      // Attr3
    VERA.data0 = SPRITE_Y_L(y);                      // Attr4
    VERA.data0 = SPRITE_Y_H(y);                      // Attr5
    VERA.data0 = SPRITE_LAYER1 | 2;                  // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_16;  // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_UP_RIGHT), sprite);   // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_UP_RIGHT);     // Attr1
    VERA.data0 = SPRITE_X_L(x);                      // Attr2
    VERA.data0 = SPRITE_X_H(x);                      // Attr3
    VERA.data0 = SPRITE_Y_L(y);                      // Attr4
    VERA.data0 = SPRITE_Y_H(y);                      // Attr5
    VERA.data0 = SPRITE_LAYER1 | 2;                  // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_16;  // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_UP_EXPLODE), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_UP_EXPLODE);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1;                       // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_16;   // Attr7

    // Vertical turret 16x8
    x += 32;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_LEFT_LEFT), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_LEFT);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                      // Attr2
    VERA.data0 = SPRITE_X_H(x);                      // Attr3
    VERA.data0 = SPRITE_Y_L(y);                      // Attr4
    VERA.data0 = SPRITE_Y_H(y);                      // Attr5
    VERA.data0 = SPRITE_LAYER1 | 1;                  // Attr6
    VERA.data0 = SPRITE_WIDTH_16 | SPRITE_HEIGHT_8;  // Attr7

    x += 24;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_LEFT_UP), sprite);    // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_UP);      // Attr1
    VERA.data0 = SPRITE_X_L(x);                      // Attr2
    VERA.data0 = SPRITE_X_H(x);                      // Attr3
    VERA.data0 = SPRITE_Y_L(y);                      // Attr4
    VERA.data0 = SPRITE_Y_H(y);                      // Attr5
    VERA.data0 = SPRITE_LAYER1 | 1;                  // Attr6
    VERA.data0 = SPRITE_WIDTH_16 | SPRITE_HEIGHT_8;  // Attr7

    x += 24;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_LEFT_RIGHT), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_RIGHT);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                       // Attr2
    VERA.data0 = SPRITE_X_H(x);                       // Attr3
    VERA.data0 = SPRITE_Y_L(y);                       // Attr4
    VERA.data0 = SPRITE_Y_H(y);                       // Attr5
    VERA.data0 = SPRITE_LAYER1 | 1;                   // Attr6
    VERA.data0 = SPRITE_WIDTH_16 | SPRITE_HEIGHT_8;   // Attr7

    x += 24;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(TURRET_LEFT_EXPLODE), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_EXPLODE);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                         // Attr2
    VERA.data0 = SPRITE_X_H(x);                         // Attr3
    VERA.data0 = SPRITE_Y_L(y);                         // Attr4
    VERA.data0 = SPRITE_Y_H(y);                         // Attr5
    VERA.data0 = SPRITE_LAYER1;                         // Attr6
    VERA.data0 = SPRITE_WIDTH_16 | SPRITE_HEIGHT_8;     // Attr7

    // Projectiles 8x8
    x = 16;
    y += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(QUAD_BEAM_IMAGE), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(QUAD_BEAM_IMAGE);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                     // Attr2
    VERA.data0 = SPRITE_X_H(x);                     // Attr3
    VERA.data0 = SPRITE_Y_L(y);                     // Attr4
    VERA.data0 = SPRITE_Y_H(y);                     // Attr5
    VERA.data0 = SPRITE_LAYER1;                     // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_8;  // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(QUAD_BEAM_IMAGE), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(QUAD_BEAM_IMAGE);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                     // Attr2
    VERA.data0 = SPRITE_X_H(x);                     // Attr3
    VERA.data0 = SPRITE_Y_L(y);                     // Attr4
    VERA.data0 = SPRITE_Y_H(y);                     // Attr5
    VERA.data0 = SPRITE_LAYER1 | 1;                 // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_8;  // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(DUAL_VBEAM_IMAGE), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(DUAL_VBEAM_IMAGE);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                      // Attr2
    VERA.data0 = SPRITE_X_H(x);                      // Attr3
    VERA.data0 = SPRITE_Y_L(y);                      // Attr4
    VERA.data0 = SPRITE_Y_H(y);                      // Attr5
    VERA.data0 = SPRITE_LAYER1;                      // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_8;   // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(DUAL_HBEAM_IMAGE), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(DUAL_HBEAM_IMAGE);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                      // Attr2
    VERA.data0 = SPRITE_X_H(x);                      // Attr3
    VERA.data0 = SPRITE_Y_L(y);                      // Attr4
    VERA.data0 = SPRITE_Y_H(y);                      // Attr5
    VERA.data0 = SPRITE_LAYER1;                      // Attr6
    VERA.data0 = SPRITE_WIDTH_8 | SPRITE_HEIGHT_8;   // Attr7

    // Enable sprites
    vera_sprites_enable(1);

    while (1) {
        waitvsync();
    }

    return result;
}