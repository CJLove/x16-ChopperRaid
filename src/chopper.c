#include "chopper.h"
#include "keys.h"
#include "screen.h"
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

// Chopper sequences
enum chopper_seq_e { CHOPPER_FULL_LEFT, CHOPPER_LEFT, CHOPPER_CENTER, CHOPPER_RIGHT, CHOPPER_FULL_RIGHT };

#define MAX_SEQUENCE 5
#define SEQ_LENGTH 4
#define IMAGE_ADDR_L(addr) (uint8_t)((addr >> 5) & 0xff)
#define IMAGE_ADDR_H(addr) (uint8_t)((addr >> 13) & 0xf)

static const int gravityDeltaY = 1;

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

static void debugChopper()
{
    int x = 0;
    char buffer[40];
    int len = sprintf(buffer,"%c:%4d %c:%4d %c:%4d %c:%4d",
                      24,chopper.x, 25, chopper.y, 8, chopper.hscroll, 22, chopper.vscroll);
    setBase(LAYER1_MAP_BASE);
    for (x = 0; x < len; x++) {
        setTile(x,0,buffer[x],0);
    }
}

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
                    deltaX = 3;
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
                    deltaX = -3;
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
        if (action & KEY_A) {
            chopper.hscroll = 0;
            VERA.layer0.hscroll = chopper.hscroll;
            deltaX = 0;
        }
        if (action & KEY_B) {
            chopper.hscroll = 4095;
            VERA.layer0.hscroll = chopper.hscroll;
            deltaX = 0;
        }
        if ((chopper.ticks % 8) == 0) {
            deltaY += gravityDeltaY;
        }
        if (action & KEY_UP) {
            deltaY = -1;
        }
        else if (action & KEY_DOWN) {
            deltaY += 1;
        }

        if (deltaX > 0) {
            if (chopper.x >= CHOPPER_RIGHT_THRESH) {
                chopper.hscroll += deltaX;
                VERA.layer0.hscroll = chopper.hscroll;
            }
            else {
                chopper.x += deltaX;
            }
        }
        else if (deltaX < 0) {
            if (chopper.x <= CHOPPER_LEFT_THRESH) {
                chopper.hscroll += deltaX;
                VERA.layer0.hscroll = chopper.hscroll;
            }
            else {
                chopper.x += deltaX;
            }
        }
        if (deltaY > 0) {
            if (chopper.y >= CHOPPER_BOTTOM_THRESH) {
                chopper.vscroll++;
                VERA.layer0.vscroll = chopper.vscroll;
            }
            else {
                chopper.y += deltaY;
            }
        }
        else if (deltaY < 0) {
            if (chopper.y <= CHOPPER_TOP_THRESH) {
                chopper.vscroll--;
                VERA.layer0.vscroll = chopper.vscroll;
            }
            else {
                chopper.y += deltaY;
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
        debugChopper();
    }
}