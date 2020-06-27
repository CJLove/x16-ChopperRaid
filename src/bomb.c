#include "bomb.h"
#include "chopper.h"
#include "screen.h"
#include <cx16.h>
#include <vera.h>
#include <stdio.h>

#define MAX_BOMBS 4

// Base sprite attributes for bombs (sprites 41-44)

#define BOMB_ATTR0 0x11fd48
#define BOMB_ATTR1 0x11fd49
#define BOMB_ATTR2 0x11fd4a
#define BOMB_ATTR3 0x11fd4b
#define BOMB_ATTR4 0x11fd4c
#define BOMB_ATTR5 0x11fd4d
#define BOMB_ATTR6 0x11fd4e
#define BOMB_ATTR7 0x11fd4f

// Tiles of interest to bombs
#define TILE_BLANK 32
#define TILE_BLOCK 37
#define TILE_BARREL 41

static struct bomb_t bombs[MAX_BOMBS];

static uint8_t bombDeltaY = 4;

void initBombs(void)
{
    uint8_t i = 0;
    for (i = 0; i < MAX_BOMBS; i++) {
        bombs[i].x = 0;
        bombs[i].y = 0;
        bombs[i].spriteOffset = 8 * i;
        bombs[i].active = 0;

        vpoke(SPRITE_ADDR_L(BOMB_IMAGE), BOMB_ATTR0 + bombs[i].spriteOffset);         // Attr 0
        VERA.data0 = SPRITE_ADDR_H(BOMB_IMAGE);                                       // Attr 1
        vpoke(SPRITE_HEIGHT_8 | SPRITE_WIDTH_8, BOMB_ATTR7 + bombs[i].spriteOffset);  // Attr 7
    }
}

void dropBomb(void)
{
    uint8_t i = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    for (i = 0; i < MAX_BOMBS; i++) {
        struct bomb_t *bomb = &bombs[i];
        if (!bomb->active) {
            bomb->y = chopper.y + chopper.vscroll + 16;
            bomb->x = chopper.x + chopper.hscroll;
            switch (chopper.sequence) {
                case CHOPPER_FULL_LEFT:
                case CHOPPER_LEFT:
                    bomb->x += 24;
                    break;
                case CHOPPER_CENTER:
                    bomb->x += 32;
                    break;
                case CHOPPER_RIGHT:
                case CHOPPER_FULL_RIGHT:
                    bomb->x += 40;
                    break;
                default:
                    return;
            }

            x = bomb->x - chopper.hscroll;
            y = bomb->y - chopper.vscroll;
            vpoke(SPRITE_X_L(x), BOMB_ATTR2 + bomb->spriteOffset);  // Attr 2
            VERA.data0 = SPRITE_X_H(x);                             // Attr 3
            VERA.data0 = SPRITE_Y_L(y);                             // Attr 4
            VERA.data0 = SPRITE_Y_H(y);                             // Attr 5
            VERA.data0 = SPRITE_LAYER1;                             // Attr 6

            bomb->active = 1;
            return;
        }
    }
}

void checkTileCollision(uint8_t x, uint8_t y, struct bomb_t *bomb)
{
    
    uint8_t tile = 0;
    setBase(LAYER0_MAP_BASE);
    tile = getMetaTile(x, y);
    // Check for tiles which can be destroyed by bombs
    if (tile >= TILE_BLOCK && tile <= TILE_BARREL) {
        // Clear the tile from the meta map and actual tilemap
        setMetaTile(x, y, TILE_BLANK);
        setTile(x, y, TILE_BLANK, 0);

        // TODO: Update score

        // Turn off sprite and mark bomb as inactive
        vpoke(0, BOMB_ATTR6 + bomb->spriteOffset);  // Attr 6
        bomb->active = 0;
    }
    else if (tile != TILE_BLANK) {
        // Turn off sprite and mark bomb as inactive
        vpoke(0, BOMB_ATTR6 + bomb->spriteOffset);  // Attr 6
        bomb->active = 0;
    }
}

#if defined(DEBUG_CHOPPER) && defined(DEBUG_BOMBS)
void debugBomb(struct bomb_t *bomb, uint8_t y)
{
    uint8_t i = 0;
    char buf[40];
    uint8_t tx = bomb->x / 8;
    uint8_t ty = bomb->y / 8;
    uint8_t tile = getMetaTile(tx,ty);
    uint8_t len = sprintf(buf,"%d %c:%04d %c:%03d %3d %2d %c:%d",bomb->active,24,bomb->x,25,bomb->y,tx,ty,20,tile);

    setBase(LAYER1_MAP_BASE);
    for (i = 0; i < len; i++) {
        setTile(10+i,2+y,buf[i],0);
    }

}
#endif

void updateBombs(void)
{
    uint8_t i = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    uint8_t tx = 0;
    uint8_t ty = 0;
    for (i = 0; i < MAX_BOMBS; i++) {
        struct bomb_t *bomb = &bombs[i];
#if defined(DEBUG_CHOPPER) && defined(DEBUG_BOMBS)
        debugBomb(bomb,i);
#endif                
        if (bomb->active) {
            // Move the bomb
            bomb->y += bombDeltaY;
            y = bomb->y - chopper.vscroll;
            x = bomb->x - chopper.hscroll;
            vpoke(SPRITE_X_L(x), BOMB_ATTR2 + bomb->spriteOffset);  // Attr 2
            VERA.data0 = SPRITE_X_H(x);                             // Attr 3
            VERA.data0 = SPRITE_Y_L(y);                             // Attr 5
            VERA.data0 = SPRITE_Y_H(y);                             // Attr 6

            // Check for bomb going off the edge
            if (y >= 512) {
                // Disable bomb and mark as inactive
                vpoke(0, BOMB_ATTR6 + bomb->spriteOffset);  // Attr 6
                bomb->active = 0;
            }

            // Check for tile collisions
            tx = bomb->x / 8;
            ty = bomb->y / 8;
            checkTileCollision(tx, ty, bomb);
        }
    }
}