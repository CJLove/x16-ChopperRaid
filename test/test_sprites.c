#include "chopper.h"
#include "screen.h"
#include <conio.h>
#include <cx16.h>
#include <stdio.h>
#include <unistd.h>

// Test application which displays all sprites
extern int loadFiles();

#define IMAGE_ADDR_L(addr) (uint8_t)((addr >> 5) & 0xff)
#define IMAGE_ADDR_H(addr) (uint8_t)((addr >> 13) & 0xf)

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

    screenConfig();
#if 0    
    VERA.control = 0;
    //    vera_layer_enable(0x1); // Enable layer 1, disable layer 2
    VERA.display.video |= 0x20;  // Layer 1 enable

    VERA.display.hscale = 128;  // HSCALE=1
    VERA.display.vscale = 128;  // VSCALE=1
    VERA.layer0.config = 0x0;   // Disable Layer 0

    VERA.layer1.config = 0xa1;    // 128x64 map, color depth 1
    VERA.layer1.mapbase = 0x0;    // Map base at 0x00000
    VERA.layer1.tilebase = 0x80;  // Tile base at 0x10000, 8x8 tiles

    videomode(VIDEOMODE_40x30);

    // Clear the full tilemap
    // for (x = 0; x < 128; x++) {
    //     for (y = 0; y < 30; y++) {
    //         setTile(x,y,TILE_BLANK,0);
    //     }
    // }
    videomode(VIDEOMODE_40x30);
    VERA.display.hscale = 128;  // HSCALE=1
    VERA.display.vscale = 128;  // VSCALE=1
#endif
    x = 16;
    y = 16;

    // Chopper sprite images
//    vpoke((CHOPPER_CENTER_1 >> 5) & 0xff, sprite);  // Attr0
    vpoke(IMAGE_ADDR_L(CHOPPER_CENTER_1), sprite);  // Attr0
    VERA.data0 = IMAGE_ADDR_H(CHOPPER_CENTER_1);  // Attr1
    VERA.data0 = x & 0xff;                          // Attr2
    VERA.data0 = x >> 8;                            // Attr3
    VERA.data0 = y & 0xff;                          // Attr4
    VERA.data0 = y >> 8;                            // Attr5
    VERA.data0 = (3 << 2);                          // Attr6 - z pos in front of layer 1
    VERA.data0 = 0xb0;                              // Attr7

    x += 70;
    sprite += 8;

    vpoke(IMAGE_ADDR_L(CHOPPER_CENTER_2), sprite);  // Attr0
    VERA.data0 = IMAGE_ADDR_H(CHOPPER_CENTER_2);  // Attr1
    VERA.data0 = x & 0xff;                          // Attr2
    VERA.data0 = x >> 8;                            // Attr3
    VERA.data0 = y & 0xff;                          // Attr4
    VERA.data0 = 7 >> 8;                            // Attr5
    VERA.data0 = (3 << 2);                          // Attr6
    VERA.data0 = 0xb0;                              // Attr7

    x += 70;
    sprite += 8;

    vpoke(IMAGE_ADDR_L(CHOPPER_CENTER_3), sprite);  // Attr0
    VERA.data0 = (CHOPPER_CENTER_3 >> 13) & 0xf;    // Attr1
    VERA.data0 = x & 0xff;                          // Attr2
    VERA.data0 = x >> 8;                            // Attr3
    VERA.data0 = y & 0xff;                          // Attr4
    VERA.data0 = 7 >> 8;                            // Attr5
    VERA.data0 = (3 << 2);                          // Attr6
    VERA.data0 = 0xb0;                              // Attr7

    x += 70;
    sprite += 8;

    vpoke((CHOPPER_RIGHT_1 >> 5) & 0xff, sprite);  // Attr0
    VERA.data0 = (CHOPPER_RIGHT_1 >> 13) & 0xf;    // Attr1
    VERA.data0 = x & 0xff;                         // Attr2
    VERA.data0 = x >> 8;                           // Attr3
    VERA.data0 = y & 0xff;                         // Attr4
    VERA.data0 = 7 >> 8;                           // Attr5
    VERA.data0 = (3 << 2);                         // Attr6
    VERA.data0 = 0xb0;                             // Attr7

    x += 70;
    sprite += 8;

    vpoke((CHOPPER_RIGHT_2 >> 5) & 0xff, sprite);  // Attr0
    VERA.data0 = (CHOPPER_RIGHT_2 >> 13) & 0xf;    // Attr1
    VERA.data0 = x & 0xff;                         // Attr2
    VERA.data0 = x >> 8;                           // Attr3
    VERA.data0 = y & 0xff;                         // Attr4
    VERA.data0 = 7 >> 8;                           // Attr5
    VERA.data0 = (3 << 2);                         // Attr6
    VERA.data0 = 0xb0;                             // Attr7

    x += 70;
    sprite += 8;

    vpoke((CHOPPER_RIGHT_3 >> 5) & 0xff, sprite);  // Attr0
    VERA.data0 = (CHOPPER_RIGHT_3 >> 13) & 0xf;    // Attr1
    VERA.data0 = x & 0xff;                         // Attr2
    VERA.data0 = x >> 8;                           // Attr3
    VERA.data0 = y & 0xff;                         // Attr4
    VERA.data0 = 7 >> 8;                           // Attr5
    VERA.data0 = (3 << 2);                         // Attr6
    VERA.data0 = 0xb0;                             // Attr7

    x += 70;
    sprite += 8;

    vpoke((CHOPPER_TILT_RIGHT_1 >> 5) & 0xff, sprite);  // Attr0
    VERA.data0 = (CHOPPER_TILT_RIGHT_1 >> 13) & 0xf;    // Attr1
    VERA.data0 = x & 0xff;                              // Attr2
    VERA.data0 = x >> 8;                                // Attr3
    VERA.data0 = y & 0xff;                              // Attr4
    VERA.data0 = 7 >> 8;                                // Attr5
    VERA.data0 = (3 << 2);                              // Attr6
    VERA.data0 = 0xb0;                                  // Attr7

    x += 70;
    sprite += 8;

    vpoke((CHOPPER_TILT_RIGHT_2 >> 5) & 0xff, sprite);  // Attr0
    VERA.data0 = (CHOPPER_TILT_RIGHT_2 >> 13) & 0xf;    // Attr1
    VERA.data0 = x & 0xff;                              // Attr2
    VERA.data0 = x >> 8;                                // Attr3
    VERA.data0 = y & 0xff;                              // Attr4
    VERA.data0 = 7 >> 8;                                // Attr5
    VERA.data0 = (3 << 2);                              // Attr6
    VERA.data0 = 0xb0;                                  // Attr7

    x += 70;
    sprite += 8;

    vpoke((CHOPPER_TILT_RIGHT_3 >> 5) & 0xff, sprite);  // Attr0
    VERA.data0 = (CHOPPER_TILT_RIGHT_3 >> 13) & 0xf;    // Attr1
    VERA.data0 = x & 0xff;                              // Attr2
    VERA.data0 = x >> 8;                                // Attr3
    VERA.data0 = y & 0xff;                              // Attr4
    VERA.data0 = 7 >> 8;                                // Attr5
    VERA.data0 = (3 << 2);                              // Attr6
    VERA.data0 = 0xb0;                                  // Attr7
#if 0
    // Guard sprite images
    x=16;
    y = 32;
    sprite += 8;

    vpoke((GUARD_1 >> 5) & 0xff, sprite); // Attr0
    VERA.data0 = (GUARD_1 >> 13) & 0xf;   // Attr1
    VERA.data0 = x & 0xff;                // Attr2
    VERA.data0 = x >> 8;                  // Attr3
    VERA.data0 = y & 0xff;                // Attr4
    VERA.data0 = 7>> 8;                   // Attr5
    VERA.data0 = (3<<2);                  // Attr6
    VERA.data0 = 0;                       // Attr7    

    x += 16;
    sprite += 8;

    vpoke((GUARD_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;               // Attr2
    VERA.data0 = x >> 8;                 // Attr3
    VERA.data0 = y & 0xff;               // Attr4
    VERA.data0 = 7>> 8;                  // Attr5
    VERA.data0 = (3<<2);                 // Attr6
    VERA.data0 = 0;                      // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_3 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_3 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;               // Attr2
    VERA.data0 = x >> 8;                 // Attr3
    VERA.data0 = y & 0xff;               // Attr4
    VERA.data0 = 7>> 8;                  // Attr5
    VERA.data0 = (3<<2);                 // Attr6
    VERA.data0 = 0;                      // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_CLIMB_1 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_CLIMB_1 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                     // Attr2
    VERA.data0 = x >> 8;                       // Attr3
    VERA.data0 = y & 0xff;                     // Attr4
    VERA.data0 = 7>> 8;                        // Attr5
    VERA.data0 = (3<<2);                       // Attr6
    VERA.data0 = 0;                            // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_CLIMB_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_CLIMB_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                     // Attr2
    VERA.data0 = x >> 8;                       // Attr3
    VERA.data0 = y & 0xff;                     // Attr4
    VERA.data0 = 7>> 8;                        // Attr5
    VERA.data0 = (3<<2);                       // Attr6
    VERA.data0 = 0;                            // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_FALLING >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_FALLING >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                     // Attr2
    VERA.data0 = x >> 8;                       // Attr3
    VERA.data0 = y & 0xff;                     // Attr4
    VERA.data0 = 7>> 8;                        // Attr5
    VERA.data0 = (3<<2);                       // Attr6
    VERA.data0 = 0;                            // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_RAPPEL_1 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_RAPPEL_1 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_RAPPEL_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_RAPPEL_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_RAPPEL_3 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_RAPPEL_3 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7
#endif
    // Enable sprites
    vera_sprites_enable(1);

    while (1) {
        waitvsync();
    }

    return result;
}