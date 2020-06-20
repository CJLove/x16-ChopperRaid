#include "door.h"
#include "screen.h"
#include "chopper.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_DOORS
static struct door_t doors[2];

static uint8_t doorCount;

#define TILE_DOOR_U  20
#define TILE_DOOR_U2 21
#define TILE_DOOR_U3 22
#define TILE_DOOR_U4 23
#define TILE_DOOR_M  24
#define TILE_DOOR_M2 25
#define TILE_DOOR_M3 26
#define TILE_DOOR_M4 27
#define TILE_DOOR_B  28
#define TILE_DOOR_B2 29
#define TILE_DOOR_B3 30
#define TILE_DOOR_B4 31
#define TILE_BLANK  32

#define DOOR_DELAY 3

enum DoorState_e { CLOSED, OPENING, OPEN };


void initDoors(void)
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t i = 0;

    doorCount = 0;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            uint8_t tile = getMetaTile(x,y);
            if (tile == TILE_DOOR_U) {
                for (i = 1; i <= 8; i++) {
                    uint8_t tile2 = getMetaTile(x,y+i);
                    if (tile2 == TILE_DOOR_B) {
                        struct door_t *door = &doors[doorCount];
                        door->state = CLOSED;
                        door->x = x;
                        door->y = y;
                        door->height = i;

                        doorCount++;
                    }
                }
            }
        }
    }
}

#if defined(DEBUG_DOORS)
static void debugDoor(struct door_t *door, uint8_t y) {
    char buf[40];
    uint8_t dx = abs((int)door->x - (int)chopper.tx);
    uint8_t dy = abs((int)door->y - (int)chopper.ty);
    int i = 0;
    int len = sprintf(buf,"%c:%3d %c:%2d %d %d %d %d   ",24,door->x,25,door->y,door->height, door->state, dx,dy);
    setBase(LAYER1_MAP_BASE);    
    for (i = 0; i < len; i++) {
        setTile(15+i,y,buf[i],0);
    }
}
#endif
void checkDoors(void)
{
    uint8_t i = 0;
    uint8_t yy = 0;
    for (i = 0; i < doorCount; i++) {
        struct door_t *door = &doors[i];
        uint8_t tile = 0;

#if defined (DEBUG_DOORS)        
        debugDoor(door,yy++);
#endif        
        // Nothing more to do if this door is open
        if (door->state == OPEN) continue;

        // Switch to OPENING if chopper is close and has a "key"
        if (door->state == CLOSED) {
            // TODO: Check if chopper has a "key"
            if ((abs((int)door->x - (int)chopper.tx) <= 10) &&
                (abs((int)door->y - (int)chopper.ty) <= 10)) {
                door->state = OPENING;
            } else {
                continue;
            }
        }
        setBase(LAYER0_MAP_BASE);
        // Door state is OPENING
        if (door->delay) {
            door->delay--;
            return;
        }
        tile = getMetaTile(door->x,door->y+door->height);
        switch (tile) {
            // Top tile in door
            case TILE_DOOR_U:
            case TILE_DOOR_U2:
            case TILE_DOOR_U3:
                tile++;
                setMetaTile(door->x,door->y,tile);
                setTile(door->x,door->y,tile,0);
                door->delay = DOOR_DELAY;
                break;
            case TILE_DOOR_U4:
                door->state = OPEN;
                break;
            // Middle tile(s) in door
            case TILE_DOOR_M:
            case TILE_DOOR_M2:
            case TILE_DOOR_M3:
                tile++;
                setMetaTile(door->x,door->y+door->height,tile);
                setTile(door->x,door->y+door->height,tile,0);
                door->delay = DOOR_DELAY;
                break;
            case TILE_DOOR_M4:
                setMetaTile(door->x,door->y+door->height,TILE_BLANK);
                setTile(door->x,door->y+door->height,TILE_BLANK,0);
                door->height--;
                door->delay = DOOR_DELAY;
                break;
            // Bottom tile in door
            case TILE_DOOR_B:
            case TILE_DOOR_B2:
            case TILE_DOOR_B3:
                tile++;
                setMetaTile(door->x,door->y+door->height,tile);
                setTile(door->x,door->y+door->height,tile,0);
                door->delay = DOOR_DELAY;
                break;
            case TILE_DOOR_B4:
                door->height--;
                door->delay = DOOR_DELAY;
                break;
        }

    }
}