#include "chopper.h"
#include "keys.h"
#include "screen.h"
#include "collision.h"
#include "vera.h"
#include <cx16.h>
#include <joystick.h>
#include <stdio.h>

// Chopper state information
struct chopper_t chopper;




#define CHOPPER_LEFT_THRESH 50
#define CHOPPER_RIGHT_THRESH 225
#define CHOPPER_TOP_THRESH 50
#define CHOPPER_BOTTOM_THRESH 175


#define MAX_SEQUENCE 5
#define SEQ_LENGTH 4



static const int gravityDeltaY = 1; //1;
static const int horizDeltaX = 4;

static uint32_t chopperSequences[MAX_SEQUENCE][SEQ_LENGTH] = {
    // Bit-flipped for tilt left
    {CHOPPER_TILT_RIGHT_1, CHOPPER_TILT_RIGHT_2, CHOPPER_TILT_RIGHT_3, CHOPPER_TILT_RIGHT_2},
    // Bit-flipped for left
    {CHOPPER_RIGHT_1, CHOPPER_RIGHT_2, CHOPPER_RIGHT_3, CHOPPER_RIGHT_2},
    // Center
    {CHOPPER_CENTER_1, CHOPPER_CENTER_2, CHOPPER_CENTER_3, CHOPPER_CENTER_2},
    // Right
    {CHOPPER_RIGHT_1, CHOPPER_RIGHT_2, CHOPPER_RIGHT_3, CHOPPER_RIGHT_2},
    // Tilt right
    {CHOPPER_TILT_RIGHT_1, CHOPPER_TILT_RIGHT_2, CHOPPER_TILT_RIGHT_3, CHOPPER_TILT_RIGHT_2},
};





void initChopper(uint16_t x, uint16_t y)
{
    chopper.hscroll = 0;
    chopper.vscroll = 0;

    chopper.x = x;
    chopper.y = y;

    chopper.tx = (x / 8) + 1;
    chopper.partialX = (x % 8);
    chopper.ty = y / 8;
    chopper.partialY = (y % 8);

    chopper.landed = 0;

    chopper.idx = 0;
    chopper.ticks = 0;
    chopper.tiltCount = 0;

    chopper.sequence = CHOPPER_CENTER;
    chopper.direction = 0;

    // Initial sprite attribute settings
    vpoke(SPRITE_ADDR_L(CHOPPER_CENTER_1), SPRITE_ATTR0);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(CHOPPER_CENTER_1);          // Attr1
    VERA.data0 = chopper.x & 0xff;                         // Attr2
    VERA.data0 = chopper.x >> 8;                           // Attr3
    VERA.data0 = chopper.y & 0xff;                         // Attr4
    VERA.data0 = chopper.y >> 8;                           // Attr5
    VERA.data0 = SPRITE_LAYER1;                            // Attr6
    VERA.data0 = SPRITE_WIDTH_64 | SPRITE_HEIGHT_32;       // Attr7

    vera_sprites_enable(1);
}

#if defined(DEBUG_CHOPPER)
static void debugChopper()
{
    int x = 0;
    char buffer[40];
    int limit = 6 + (chopper.partialX != 0);

    int len = sprintf(buffer,"%c:%3d %c:%3d %c:%3d %c:%3d %c%c:%3d %c:%d %c%c:%2d",
                      24,chopper.x, 25, chopper.y, 8, chopper.hscroll, 22, chopper.vscroll,
                      20,24,chopper.tx,16,(chopper.partialX != 0), 20,25,chopper.ty);
    setBase(LAYER1_MAP_BASE);
    for (x = 0; x < len; x++) {
        setTile(x,0,buffer[x],0);
    }

    setBase(LAYER1_MAP_BASE);
    setTile(29,1,48,0); // Row 0
    setTile(29,2,49,0); // Row 1
    setTile(29,3,50,0); // Row 2
    setTile(29,4,51,0); // Row 4
    setTile(29,5,12,0); // Landing indicator
    setTile(31,5,(chopper.landed ? 24: 32),0);
    setTile(29,6,3,0);  // Coarse collision detect
    setTile(29,7,6,0);  // Fine collision detect
    for (x = 0; x < limit; x++) {
        setTile(30+x,1,tiles[0][x],0);
        setTile(30+x,2,tiles[1][x],0);
        setTile(30+x,3,tiles[2][x],0);
        setTile(30+x,4,tiles[3][x],0);
    }
    if (chopper.partialX == 0) {
        setTile(30+6,1,32,0);
        setTile(30+6,2,32,0);
        setTile(30+6,3,32,0);
        setTile(30+6,4,32,0);
    }
}
#endif

void updateChopper()
{
    int16_t deltaY = 0;
    int16_t deltaX = 0;
    chopper.ticks++;
    if ((chopper.ticks % 3) == 0) {
        int8_t action = inputHandler();

        if (action & KEY_RIGHT) {
            switch (chopper.sequence) {
                case CHOPPER_FULL_LEFT:
                    chopper.sequence = CHOPPER_LEFT;
                    chopper.direction = 1;
                    break;
                case CHOPPER_LEFT:
                    chopper.sequence = CHOPPER_CENTER;
                    chopper.direction = 0;
                    break;
                case CHOPPER_CENTER:
                    chopper.sequence = CHOPPER_RIGHT;
                    chopper.direction = 0;
                    break;
                case CHOPPER_RIGHT:
                    chopper.sequence = CHOPPER_FULL_RIGHT;
                    chopper.direction = 0;
                    chopper.tiltCount = 13;
                    break;
                case CHOPPER_FULL_RIGHT:
                    chopper.direction = 0;
                    chopper.tiltCount = 13;
                    deltaX = horizDeltaX;
                    break;
                default:
                    break;
            }
        }
        else if (action & KEY_LEFT) {
            switch (chopper.sequence) {
                case CHOPPER_FULL_RIGHT:
                    chopper.direction = 0;
                    chopper.sequence = CHOPPER_RIGHT;
                    break;
                case CHOPPER_RIGHT:
                    chopper.direction = 0;
                    chopper.sequence = CHOPPER_CENTER;
                    break;
                case CHOPPER_CENTER:
                    chopper.direction = 1;
                    chopper.sequence = CHOPPER_LEFT;
                    break;
                case CHOPPER_LEFT:
                    chopper.direction = 1;
                    chopper.sequence = CHOPPER_FULL_LEFT;
                    chopper.tiltCount = 13;
                    break;
                case CHOPPER_FULL_LEFT:
                    chopper.direction = 1;
                    chopper.tiltCount = 13;
                    deltaX = -horizDeltaX;
                    break;
                default:
                    break;
            }
        }
        if (action == KEY_NONE) {
            chopper.tiltCount--;
            if (chopper.tiltCount == 0) {
                if (chopper.sequence == CHOPPER_FULL_RIGHT)
                    chopper.sequence = CHOPPER_RIGHT;
                if (chopper.sequence == CHOPPER_FULL_LEFT)
                    chopper.sequence = CHOPPER_LEFT;
            }
        }
        if ((chopper.ticks % 6) == 0) {
            deltaY += gravityDeltaY;
        }
        if (action & KEY_UP) {
            if (action == KEY_UP) {
                if (chopper.sequence == CHOPPER_FULL_RIGHT)
                    chopper.sequence = CHOPPER_RIGHT;
                if (chopper.sequence == CHOPPER_FULL_LEFT)
                    chopper.sequence = CHOPPER_LEFT;
            }
            deltaY = -1;
        }
        else if (action & KEY_DOWN) {
            if (action == KEY_DOWN) {
                if (chopper.sequence == CHOPPER_FULL_RIGHT)
                    chopper.sequence = CHOPPER_RIGHT;
                if (chopper.sequence == CHOPPER_FULL_LEFT)
                    chopper.sequence = CHOPPER_LEFT;
            }
            deltaY += 1;
        }

        if (deltaX > 0) {
            if (chopper.x >= CHOPPER_RIGHT_THRESH && chopper.hscroll <= 700) {
                chopper.hscroll += deltaX;
                VERA.layer0.hscroll = chopper.hscroll;
            }
            else {
                if (chopper.x <= 260) {
                    chopper.x += deltaX;
                } else {
                    deltaX = 0;
                }
            }
        }
        else if (deltaX < 0) {
            if (chopper.x <= CHOPPER_LEFT_THRESH && chopper.hscroll >= 2) {
                chopper.hscroll += deltaX;
                VERA.layer0.hscroll = chopper.hscroll;
            }
            else {
                if (chopper.x >= 2) {
                    chopper.x += deltaX;
                } else {
                    deltaX = 0;
                }
            }
        }
        chopper.landed = okToLand();
        if (deltaY > 0) {

            if (chopper.landed) {
                deltaY = 0;
            }
            if (chopper.y >= CHOPPER_BOTTOM_THRESH && chopper.vscroll <= 312) {
                chopper.vscroll++;
                VERA.layer0.vscroll = chopper.vscroll;
            }
            else {
                if (chopper.y <= 200) {
                    chopper.y += deltaY;
                } else {
                    deltaY = 0;
                }
            }
        }
        else if (deltaY < 0) {
            if (chopper.y <= CHOPPER_TOP_THRESH && chopper.vscroll >=1) {
                chopper.vscroll--;
                VERA.layer0.vscroll = chopper.vscroll;
            }
            else {
                if (chopper.y >= 1) {
                    chopper.y += deltaY;
                } else {
                    deltaY = 0;
                }
            }
        }
        chopper.tx = (chopper.x + chopper.hscroll) / 8 + 1;
        chopper.partialX = (chopper.x + chopper.hscroll) % 8;
        chopper.ty = (chopper.y + chopper.vscroll) / 8;
        chopper.partialY = (chopper.y + chopper.vscroll) % 8;
        getTiles(chopper.tx,chopper.ty);
        if (checkCoarseCollision()) {
            if (checkFineCollision()) {
                // Crash
            }
        }

        chopper.idx++;
        chopper.idx %= SEQ_LENGTH;
        vpoke(SPRITE_ADDR_L(chopperSequences[chopper.sequence][chopper.idx]), SPRITE_ATTR0);
        VERA.data0 = SPRITE_ADDR_H(chopperSequences[chopper.sequence][chopper.idx]);
        VERA.data0 = chopper.x & 0xff;
        VERA.data0 = chopper.x >> 8;
        VERA.data0 = chopper.y & 0xff;
        VERA.data0 = chopper.y >> 8;
        VERA.data0 = SPRITE_LAYER1 | chopper.direction;
#if defined(DEBUG_CHOPPER)        
        debugChopper();
#endif        
    }
}