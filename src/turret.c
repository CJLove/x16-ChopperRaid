#include "turret.h"
#include "chopper.h"
#include "screen.h"
#include "vera.h"
#include <cx16.h>
#include <stdio.h>

#define MAX_TURRETS 10

// Base sprite attributes for turrets (use sprites 1-10)
#define TURRET_ATTR0 0x11fc08
#define TURRET_ATTR1 0x11fc09
#define TURRET_ATTR2 0x11fc0a
#define TURRET_ATTR3 0x11fc0b
#define TURRET_ATTR4 0x11fc0c
#define TURRET_ATTR5 0x11fc0d
#define TURRET_ATTR6 0x11fc0e
#define TURRET_ATTR7 0x11fc0f

static struct turret_t turrets[MAX_TURRETS];
static uint8_t turretCount;

// Tiles marking the beginning/end of a turret track
#define TILE_TURRET_UL 90
#define TILE_TURRET_UR 92
#define TILE_TURRET_LL 106
#define TILE_TURRET_LR 108
// Tiles marking the turret track and identifying the orientation
#define TILE_TURRET_UP_BAR 91
#define TILE_TURRET_DOWN_BAR 107
#define TILE_TURRET_LEFT_BAR 104
#define TILE_TURRET_RIGHT_BAR 105

// Horizontal or vertical movement increment
#define TURRET_DELTA_XY 2

// Find a turret with start or end limits matching tile location x,y
// static int findTurret(uint8_t x, uint8_t y)
// {
//     uint8_t i = 0;
//     for (i = 0; i < turretCount; i++) {
//         if ((turrets[i].limit1_x == x && turrets[i].limit1_y == y) ||
//             (turrets[i].limit2_x == x && turrets[i].limit2_y == y)) {
//             return i;
//         }
//     }
//     return -1;
// }

// Initialization for a turret facing up
static int8_t handleTurretUp(uint8_t x, uint8_t y)
{
    int8_t i;

    for (i = 1; i <= 8; i++) {
        uint8_t tile = getMetaTile(x + i, y);
        if (tile == TILE_TURRET_UR) {
            turrets[turretCount].orientation = TURRET_UP;
            turrets[turretCount].limit1_x = x;
            turrets[turretCount].limit1_y = y;
            turrets[turretCount].limit2_x = x + i;
            turrets[turretCount].limit2_y = y;
            turrets[turretCount].spriteOffset = 8 * turretCount;
            turrets[turretCount].flip = 0;
            turrets[turretCount].x = x * 8 + 4;
            turrets[turretCount].y = (y * 8) - 8;
            turrets[turretCount].maxMove = (((x + i) * 8 - 4) - turrets[turretCount].x) / TURRET_DELTA_XY;
            turrets[turretCount].curMove = 0;
            turrets[turretCount].direction = 1;
            // Set the sprite image
            vpoke(SPRITE_ADDR_L(TURRET_UP_LEFT), TURRET_ATTR0 + turrets[turretCount].spriteOffset);      // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_LEFT);                                                  // Attr 1
            vpoke(SPRITE_HEIGHT_16 | SPRITE_WIDTH_8, TURRET_ATTR7 + turrets[turretCount].spriteOffset);  // Attr 7
#if defined(UNIT_TEST)
            printf("TurretUp from (%d,%d)-(%d,%d)\n", x, y, x + i, y);
#endif
            return 1;
        }
    }
    // Invalid turret configuration
#if defined(UNIT_TEST)
    printf("Invalid TurretUp starting at (%d,%d)\n", x, y);
#endif
    return 0;
}

static void moveTurretUp(struct turret_t *turret)
{
    turret->curMove++;
    if (turret->curMove < turret->maxMove) {
        turret->x += (turret->direction * TURRET_DELTA_XY);
        if (turret->curMove == turret->maxMove / 2) {
            // Change to "up" image at mid point
            vpoke(SPRITE_ADDR_L(TURRET_UP_UP), TURRET_ATTR0 + turrets->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_UP);                                  // Attr 1
        }
    }
    else {
        turret->curMove = 0;
        turret->direction = -turret->direction;
        turret->x += (turret->direction * TURRET_DELTA_XY);
        if (turret->direction == -1) {
            vpoke(SPRITE_ADDR_L(TURRET_UP_LEFT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_LEFT);                                 // Attr 1
        }
        else {
            vpoke(SPRITE_ADDR_L(TURRET_UP_RIGHT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_RIGHT);                                 // Attr 1
        }
    }

    // TODO: Targetting and shooting
}

// Initialization for a turret facing down
static int8_t handleTurretDown(uint8_t x, uint8_t y)
{
    int8_t i;

    for (i = 1; i <= 8; i++) {
        uint8_t tile = getMetaTile(x + i, y);
        if (tile == TILE_TURRET_LR) {
            turrets[turretCount].orientation = TURRET_DOWN;
            turrets[turretCount].limit1_x = x;
            turrets[turretCount].limit1_y = y;
            turrets[turretCount].limit2_x = x + i;
            turrets[turretCount].limit2_y = y;
            turrets[turretCount].spriteOffset = 8 * turretCount;
            turrets[turretCount].flip = SPRITE_VERT_FLIP;
            turrets[turretCount].x = x * 8 + 4;
            turrets[turretCount].y = y * 8;
            turrets[turretCount].maxMove = (((x + i) * 8 - 4) - turrets[turretCount].x) / TURRET_DELTA_XY;
            turrets[turretCount].curMove = 0;
            turrets[turretCount].direction = 1;
            // Set the sprite image
            vpoke(SPRITE_ADDR_L(TURRET_UP_RIGHT), TURRET_ATTR0 + turrets[turretCount].spriteOffset);     // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_RIGHT);                                                 // Attr 1
            vpoke(SPRITE_HEIGHT_16 | SPRITE_WIDTH_8, TURRET_ATTR7 + turrets[turretCount].spriteOffset);  // Attr 7

#if defined(UNIT_TEST)
            printf("TurretDown from (%d,%d)-(%d,%d)\n", x, y, x + i, y);
#endif
            return 1;
        }
    }
    // Invalid turret configuration
#if defined(UNIT_TEST)
    printf("Invalid TurretDown starting at (%d,%d)\n", x, y);
#endif
    return 0;
}

static void moveTurretDown(struct turret_t *turret)
{
    turret->curMove++;
    if (turret->curMove < turret->maxMove) {
        turret->x += (turret->direction * TURRET_DELTA_XY);
        if (turret->curMove == turret->maxMove / 2) {
            // Change to "up" image at mid point
            vpoke(SPRITE_ADDR_L(TURRET_UP_UP), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_UP);                                 // Attr 1
        }
    }
    else {
        turret->curMove = 0;
        turret->direction = -turret->direction;
        turret->x += (turret->direction * TURRET_DELTA_XY);
        if (turret->direction == -1) {
            vpoke(SPRITE_ADDR_L(TURRET_UP_RIGHT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_RIGHT);                                 // Attr 1
        }
        else {
            vpoke(SPRITE_ADDR_L(TURRET_UP_LEFT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_UP_LEFT);                                 // Attr 1
        }
    }

    // TODO: Targetting and shooting
}

// Initialization for a turret facing left
static int8_t handleTurretLeft(uint8_t x, uint8_t y)
{
    int8_t i;

    for (i = 1; i <= 8; i++) {
        uint8_t tile = getMetaTile(x, y + i);
        if (tile == TILE_TURRET_LL) {
            turrets[turretCount].orientation = TURRET_LEFT;
            turrets[turretCount].limit1_x = x;
            turrets[turretCount].limit1_y = y;
            turrets[turretCount].limit2_x = x;
            turrets[turretCount].limit2_y = y + i;
            turrets[turretCount].spriteOffset = 8 * turretCount;
            turrets[turretCount].flip = 0;
            turrets[turretCount].x = x * 8 - 8;
            turrets[turretCount].y = y * 8 + 4;
            turrets[turretCount].maxMove = (((y + i) * 8 - 4) - turrets[turretCount].y) / TURRET_DELTA_XY;
            turrets[turretCount].curMove = 0;
            turrets[turretCount].direction = 1;
            // Set the sprite image
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_LEFT), TURRET_ATTR0 + turrets[turretCount].spriteOffset);    // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_LEFT);                                                // Attr 1
            vpoke(SPRITE_WIDTH_16 | SPRITE_HEIGHT_8, TURRET_ATTR7 + turrets[turretCount].spriteOffset);  // Attr 7

#if defined(UNIT_TEST)
            printf("TurretLeft from (%d,%d)-(%d,%d)\n", x, y, x, y + i);
#endif
            return 1;
        }
    }
#if defined(UNIT_TEST)
    printf("Invalid TurretLeft starting at (%d,%d)\n", x, y);
#endif
    return 0;
}

static void moveTurretLeft(struct turret_t *turret)
{
    turret->curMove++;
    if (turret->curMove < turret->maxMove) {
        turret->y += (turret->direction * TURRET_DELTA_XY);
        if (turret->curMove == (turret->maxMove / 2)) {
            // Move to "up" angle at midpoint
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_UP), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_UP);                                 // Attr 1
        }
    }
    else {
        turret->curMove = 0;
        turret->direction = -turret->direction;
        turret->y += (turret->direction * TURRET_DELTA_XY);
        if (turret->direction == -1) {
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_RIGHT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_RIGHT);                                 // Attr 1
        } else {
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_LEFT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_LEFT);                                 // Attr 1            
        }
    }

    // TODO: Targetting and shooting
}

// Initialization for a turret facing right
static int8_t handleTurretRight(uint8_t x, uint8_t y)
{
    int8_t i;

    for (i = 1; i <= 8; i++) {
        uint8_t tile = getMetaTile(x, y + i);
        if (tile == TILE_TURRET_LR) {
            turrets[turretCount].orientation = TURRET_RIGHT;
            turrets[turretCount].limit1_x = x;
            turrets[turretCount].limit1_y = y;
            turrets[turretCount].limit2_x = x;
            turrets[turretCount].limit2_y = y + i;
            turrets[turretCount].spriteOffset = 8 * turretCount;
            turrets[turretCount].flip = SPRITE_HORIZ_FLIP;
            turrets[turretCount].x = x * 8;
            turrets[turretCount].y = y * 8 + 4;
            turrets[turretCount].maxMove = (((y + i) * 8 - 4) - turrets[turretCount].y) / TURRET_DELTA_XY;
            turrets[turretCount].curMove = 0;
            turrets[turretCount].direction = 1;
            // Set the sprite image
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_LEFT), TURRET_ATTR0 + turrets[turretCount].spriteOffset);    // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_LEFT);                                                // Attr 1
            vpoke(SPRITE_WIDTH_16 | SPRITE_HEIGHT_8, TURRET_ATTR7 + turrets[turretCount].spriteOffset);  // Attr 7

#if defined(UNIT_TEST)
            printf("TurretRight from (%d,%d)-(%d,%d)\n", x, y, x, y + i);
#endif
            return 1;
        }
    }
#if defined(UNIT_TEST)
    printf("Invalid TurretRight starting at (%d,%d)\n", x, y);
#endif
    return 0;
}

static void moveTurretRight(struct turret_t *turret)
{
    turret->curMove++;
    if (turret->curMove < turret->maxMove) {
        turret->y += (turret->direction * TURRET_DELTA_XY);
        if (turret->curMove == (turret->maxMove / 2)) {
            // Move to "up" angle at midpoint
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_UP), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_UP);                                 // Attr 1
        }        
    }
    else {
        turret->curMove = 0;
        turret->direction = -turret->direction;
        turret->y += (turret->direction * TURRET_DELTA_XY);
        if (turret->direction == -1) {
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_LEFT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_LEFT);                                 // Attr 1
        } else {
            vpoke(SPRITE_ADDR_L(TURRET_LEFT_RIGHT), TURRET_ATTR0 + turret->spriteOffset);  // Attr 0
            VERA.data0 = SPRITE_ADDR_H(TURRET_LEFT_RIGHT);                                 // Attr 1            
        }        
    }

    // TODO: Targetting and shooting
}

int initTurrets()
{
    uint8_t y = 0;
    uint8_t x = 0;
    uint8_t i = 0;

    turretCount = 0;

    // Scan the meta tilemap for corner tiles
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            uint8_t tile = getMetaTile(x, y);
            int8_t ok = -1;
            switch (tile) {
                case TILE_TURRET_UL:
                    if (getMetaTile(x + 1, y) == TILE_TURRET_UP_BAR) {
                        ok = handleTurretUp(x, y);
                    }
                    else if (getMetaTile(x, y + 1) == TILE_TURRET_LEFT_BAR) {
                        ok = handleTurretLeft(x, y);
                    }
                    break;
                case TILE_TURRET_LL:
                    if (getMetaTile(x + 1, y) == TILE_TURRET_DOWN_BAR) {
                        ok = handleTurretDown(x, y);
                    }
                case TILE_TURRET_UR:
                    if (getMetaTile(x, y + 1) == TILE_TURRET_RIGHT_BAR) {
                        ok = handleTurretRight(x, y);
                    }
                    break;
                default:
                    break;
            }
            if (ok == 1) {
                // Common turret initialization
                turrets[turretCount].active = 1;
                turrets[turretCount].angle = ANGLE_LEFT;
                turrets[turretCount].damageCount = 0;
                turrets[turretCount].direction = 1;  // RIGHT/DOWN

                turretCount++;
            }
            else if (ok == 0) {
                // Invalid turret configuration
                return 0;
            }
        }
    }

    for (i = turretCount; i < MAX_TURRETS; i++) {
        turrets[i].active = 0;
    }
    return 1;
}

#if defined(DEBUG_TURRETS)
static void debugTurrets(struct turret_t *turret, uint8_t y)
{
    char buf[40];
    int i = 0;
    int visible = (chopper.hscroll <= turret->x && chopper.hscroll + 320 >= turret->x) &&
                  (chopper.vscroll <= turret->y && chopper.vscroll + 200 >= turret->y);
    int len =
        sprintf(buf, "%c:%04u %c:%03u %d %2d/%2d", 24, turret->x, 25, turret->y, visible, turret->curMove, turret->maxMove);
    for (i = 0; i < len; i++) {
        setTile(18 + i, y, buf[i], 0);
    }
}
#endif

void updateTurrets(void)
{
    int i = 0;
    uint8_t yy = 0;
    for (i = 0; i < turretCount; i++) {
        struct turret_t *turret = &turrets[i];

        setBase(LAYER1_MAP_BASE);

        // If turret is active and located within the visible portal
        if (turret->active) {
            if ((chopper.hscroll <= turret->x && (chopper.hscroll + 320) >= turret->x) &&
                (chopper.vscroll <= turret->y && (chopper.vscroll + 240) >= turret->y)) {
                uint16_t x;
                uint16_t y;

                // TODO: Turret movement and targeting
                switch (turret->orientation) {
                    case TURRET_UP:
                        moveTurretUp(turret);
                        break;
                    case TURRET_DOWN:
                        moveTurretDown(turret);
                        break;
                    case TURRET_LEFT:
                        moveTurretLeft(turret);
                        break;
                    case TURRET_RIGHT:
                        moveTurretRight(turret);
                        break;
                }

                // Calculate x and y relative to hscroll/vscroll
                x = turret->x - chopper.hscroll;
                y = turret->y - chopper.vscroll;

                vpoke(SPRITE_X_L(x), TURRET_ATTR2 + turret->spriteOffset);  // Attr 2
                VERA.data0 = SPRITE_X_H(x);                                 // Attr 3
                VERA.data0 = SPRITE_Y_L(y);                                 // Attr 4
                VERA.data0 = SPRITE_Y_H(y);                                 // Attr 5
                VERA.data0 = SPRITE_LAYER1 | turret->flip;                  // Attr 6
            }
            else {
                // Turn sprite off
                vpoke(0, TURRET_ATTR6 + turret->spriteOffset);
            }

#if defined(DEBUG_TURRETS)
            debugTurret(turret, yy++);
#endif
        }
        else {
            // Turn sprite off
            vpoke(0, TURRET_ATTR6 + turret->spriteOffset);
#if defined(DEBUG_TURRETS)
            debugTurret(turret, yy++);
#endif
        }
    }
}