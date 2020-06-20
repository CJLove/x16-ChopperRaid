#include "beams.h"
#include "chopper.h"
#include "screen.h"
#include <cx16.h>
#include <stdio.h>
#include <vera.h>

// Base sprite attributes for quad beams (use sprites 11-30)
#define QUAD_ATTR0 0x11fc58
#define QUAD_ATTR1 0x11fc59
#define QUAD_ATTR2 0x11fc5a
#define QUAD_ATTR3 0x11fc5b
#define QUAD_ATTR4 0x11fc5c
#define QUAD_ATTR5 0x11fc5d
#define QUAD_ATTR6 0x11fc5e
#define QUAD_ATTR7 0x11fc5f

// Base sprite attributes for dual beams (use sprites 31-40)
#define DUAL_ATTR0 0x11fcf8
#define DUAL_ATTR1 0x11fcf9
#define DUAL_ATTR2 0x11fcfa
#define DUAL_ATTR3 0x11fcfb
#define DUAL_ATTR4 0x11fcfc
#define DUAL_ATTR5 0x11fcfd
#define DUAL_ATTR6 0x11fcfe
#define DUAL_ATTR7 0x11fcff

// Tiles for identifying location of quad beams
#define TILE_QUAD_UL 86
#define TILE_QUAD_UR 87
#define TILE_QUAD_LL 88
#define TILE_QUAD_LR 89

// Tiles for identifying location of dual beams
#define TILE_DUAL_L 35
#define TILE_DUAL_R 36

#define TILE_DUAL_T 33
#define TILE_DUAL_B 34

#define MAX_QUAD_BEAMS 10
#define MAX_DUAL_BEAMS 10

static struct quad_beam_t quadBeams[MAX_QUAD_BEAMS];
static struct dual_beam_t dualBeams[MAX_DUAL_BEAMS];

static uint8_t quadBeamCount;
static uint8_t dualBeamCount;

#define BEAM_DELTA_XY 4

#define BEAM_DELAY 80

#if defined(DEBUG_BEAMS)
static void debugQuad(struct quad_beam_t *quad, uint8_t y)
{
    char buf[40];
    int i = 0;
    int visible = ((chopper.hscroll <= quad->x1 && (chopper.hscroll + 320) >= quad->x2) &&
                   (chopper.vscroll <= quad->y1 && (chopper.vscroll + 240) >= quad->y1));

    int len = sprintf(buf, "%c:%04d %c:%03d %d %2d/%2d", 24, quad->x1, 25, quad->y1, visible, quad->curMove, quad->maxMoves);
    for (i = 0; i < len; i++) {
        setTile(20 + i, y, buf[i], 0);
    }
}
static void debugDual(struct dual_beam_t *dual, uint8_t y)
{
    char buf[40];
    int i;
    int visible = 0;

    int len = sprintf(buf, "%c:%04d %c:%03d %d %2d/%2d", 24, dual->x, 25, dual->y, visible, dual->curMove, dual->maxMoves);
    for (i = 0; i < len; i++) {
        setTile(20 + i, y, buf[i], 0);
    }
}
#endif

static int handleQuadBeams(uint8_t x, uint8_t y)
{
    uint8_t i = 0;
    for (i = 0; i <= 12; i++) {
        uint8_t tile = getMetaTile(x + i, y);
        if (tile == TILE_QUAD_UR) {
            uint8_t tile2 = getMetaTile(x, y + i);
            uint8_t tile3 = getMetaTile(x + i, y + i);
            if (tile2 == TILE_QUAD_LL && tile3 == TILE_QUAD_LR) {
                struct quad_beam_t *quad = &quadBeams[quadBeamCount];
                quad->active = 1;
                quad->limit1_x = x;
                quad->limit1_y = y;
                quad->limit2_x = x + i;
                quad->limit2_y = y;
                quad->x1 = x * 8 + 4;
                quad->y1 = y * 8 + 4;
                quad->x2 = (x + i) * 8 - 4;
                quad->y2 = y * 8 + 4;
                quad->maxMoves = (((x + i) * 8 - 4) - quad->x1) / BEAM_DELTA_XY;
                quad->spriteOffset1 = 8 * quadBeamCount;
                quad->spriteOffset2 = 8 * (quadBeamCount + MAX_QUAD_BEAMS);

                // Set sprite images
                vpoke(SPRITE_ADDR_L(QUAD_BEAM_IMAGE), QUAD_ATTR0 + quad->spriteOffset1);    // Attr 0
                VERA.data0 = SPRITE_ADDR_H(QUAD_BEAM_IMAGE);                                // Attr 1
                vpoke(SPRITE_HEIGHT_8 | SPRITE_WIDTH_8, QUAD_ATTR7 + quad->spriteOffset1);  // Attr 7

                vpoke(SPRITE_ADDR_L(QUAD_BEAM_IMAGE), QUAD_ATTR0 + quad->spriteOffset2);    // Attr 0
                VERA.data0 = SPRITE_ADDR_H(QUAD_BEAM_IMAGE);                                // Attr 1
                vpoke(SPRITE_HEIGHT_8 | SPRITE_WIDTH_8, QUAD_ATTR7 + quad->spriteOffset2);  // Attr 7

#if defined(DEBUG_BEAMS)
//                printf("Quad beams anchored at (%d,%d) - (%d,%d)\n",x,y,x+i,y);
#endif
                quadBeamCount++;
                return 1;
            }
            // Invalid quad beam configuration
#if defined(DEBUG_BEAMS)
//            printf("Invalid quad beam anchored at (%d,%d)\n",x,y);
#endif
            return 0;
        }
    }
    // Invalid quad beam configuration
#if defined(DEBUG_BEAMS)
//    printf("Invalid quad beam anchored at (%d,%d)\n",x,y);
#endif
    return 0;
}

static void moveQuadBeams(struct quad_beam_t *quad)
{
    if (quad->delay) {
        quad->delay--;
        return;
    }
    quad->curMove++;
    if (quad->curMove < quad->maxMoves) {
        quad->x1 += BEAM_DELTA_XY;
        quad->y1 += BEAM_DELTA_XY;

        quad->x2 -= BEAM_DELTA_XY;
        quad->y2 += BEAM_DELTA_XY;
    }
    else {
        quad->curMove = 0;
        quad->x1 = quad->limit1_x * 8 + 4;
        quad->y1 = quad->limit1_y * 8 + 4;
        quad->x2 = quad->limit2_x * 8 - 4;
        quad->y2 = quad->limit2_y * 8 + 4;
        quad->delay = BEAM_DELAY;
    }
}

static int handleDualHorizBeams(uint8_t x, uint8_t y)
{
    int8_t i = 0;
    for (i = 1; i <= 12; i++) {
        uint8_t tile = getMetaTile(x + i, y);
        if (tile == TILE_DUAL_R) {
            struct dual_beam_t *dual = &dualBeams[dualBeamCount];
            dual->active = 1;
            dual->limit1_x = x;
            dual->limit1_y = y;
            dual->limit2_x = x + i;
            dual->limit2_y = y;
            dual->x = x * 8 + 8;
            dual->y = y * 8;
            dual->orientation = 1;
            dual->maxMoves = (((x + i) * 8) - dual->x) / BEAM_DELTA_XY;
            dual->spriteOffset = dualBeamCount * 8;
            // Set sprite images
            vpoke(SPRITE_ADDR_L(DUAL_HBEAM_IMAGE), DUAL_ATTR0 + dual->spriteOffset);   // Attr 0
            VERA.data0 = SPRITE_ADDR_H(DUAL_HBEAM_IMAGE);                              // Attr 1
            vpoke(SPRITE_HEIGHT_8 | SPRITE_WIDTH_8, DUAL_ATTR7 + dual->spriteOffset);  // Attr 7
#if defined(DEBUG_BEAMS)
            printf("DualHorizBeam from (%d,%d) - (%d-%d)\n", x, y, x + i, y);
#endif            
            dualBeamCount++;
            return 1;
        } 
    }
    // Invalid dual beam
#if defined(DEBUG_BEAMS)    
    printf("Invalid DualHorizBeam at (%d,%d)\n",x,y);
#endif    
    return 0;
}

static int handleDualVertBeams(uint8_t x, uint8_t y)
{
    int8_t i = 0;
    for (i = 1; i <= 12; i++) {
        uint8_t tile = getMetaTile(x, y + i);
        if (tile == TILE_DUAL_B) {
            struct dual_beam_t *dual = &dualBeams[dualBeamCount];
            dual->active = 1;
            dual->limit1_x = x;
            dual->limit1_y = y;
            dual->limit2_x = x;
            dual->limit2_y = y + i;
            dual->x = x * 8;
            dual->y = y * 8 + 8;
            dual->orientation = 0;
            dual->maxMoves = (((y + i) * 8) - dual->y) / BEAM_DELTA_XY;
            dual->spriteOffset = dualBeamCount * 8;
            // Set sprite images
            vpoke(SPRITE_ADDR_L(DUAL_VBEAM_IMAGE), DUAL_ATTR0 + dual->spriteOffset);   // Attr 0
            VERA.data0 = SPRITE_ADDR_H(DUAL_VBEAM_IMAGE);                              // Attr 1
            vpoke(SPRITE_HEIGHT_8 | SPRITE_WIDTH_8, DUAL_ATTR7 + dual->spriteOffset);  // Attr 7
#if defined(DEBUG_BEAMS)
            printf("DualVertBeam from (%d,%d) - (%d-%d)\n", x, y, x, y + i);
#endif            
            dualBeamCount++;
            return 1;
        }
    }
    // Invalid dual beam
#if defined(DEBUG_BEAMS)    
    printf("Invalid DualVertBeam at (%d,%d)\n",x,y);
#endif    
    return 0;
}

static void moveDualBeam(struct dual_beam_t *dual)
{
    if (dual->delay) {
        dual->delay--;
        return;
    }
    dual->curMove++;
    if (dual->curMove < dual->maxMoves) {
        if (dual->orientation) {
            dual->x += BEAM_DELTA_XY;
        }
        else {
            dual->y += BEAM_DELTA_XY;
        }
    }
    else {
        dual->curMove = 0;
        if (dual->orientation) {
            dual->x = dual->limit1_x * 8 + 8;
            dual->y = dual->limit1_y * 8;
        }
        else {
            dual->x = dual->limit1_x * 8;
            dual->y = dual->limit1_y * 8 + 8;
        }
        dual->delay = BEAM_DELAY;
    }
}

int initBeams()
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t i = 0;

    quadBeamCount = 0;
    dualBeamCount = 0;

    // Scan the meta tilemap for key tiles
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            uint8_t tile = getMetaTile(x, y);
            int8_t ok = -1;
            switch (tile) {
                case TILE_QUAD_UL:
                    ok = handleQuadBeams(x, y);
                    break;
                case TILE_DUAL_L:
                    ok = handleDualHorizBeams(x, y);
                    break;
                case TILE_DUAL_T:
                    ok = handleDualVertBeams(x, y);
                    break;
            }
            // if (ok == 0) {
            //     // Invalid beam configuration
            //     return 0;
            // }
        }
    }

    for (i = quadBeamCount; i < MAX_QUAD_BEAMS; i++) {
        quadBeams[i].active = 0;
    }
    for (i = dualBeamCount; i < MAX_DUAL_BEAMS; i++) {
        dualBeams[i].active = 0;
    }

    return 1;
}

void updateBeams()
{
    int i = 0;
    uint8_t yy = 0;
    for (i = 0; i < quadBeamCount; i++) {
        struct quad_beam_t *quad = &quadBeams[i];
        if (quad->active) {
            moveQuadBeams(quad);

#if defined(DEBUG_BEAMS)
            debugQuad(quad,yy++);
#endif
            if ((chopper.hscroll <= quad->x1 && (chopper.hscroll + 320) >= quad->x2) &&
                (chopper.vscroll <= quad->y1 && (chopper.vscroll + 240) >= quad->y1)) {
                uint16_t x;
                uint16_t y;

                if (quad->delay) {
                    // Turn sprites off
                    vpoke(0, QUAD_ATTR6 + quad->spriteOffset1);
                    vpoke(0, QUAD_ATTR6 + quad->spriteOffset2);
                }
                else {

                    // Calculate beam x y positions relative to hscroll/vscroll
                    x = quad->x1 - chopper.hscroll;
                    y = quad->y1 - chopper.vscroll;

                    vpoke(SPRITE_X_L(x), QUAD_ATTR2 + quad->spriteOffset1);  // Attr2
                    VERA.data0 = SPRITE_X_H(x);                              // Attr3
                    VERA.data0 = SPRITE_Y_L(y);                              // Attr4
                    VERA.data0 = SPRITE_Y_H(y);                              // Attr5
                    VERA.data0 = SPRITE_LAYER1;                              // Attr6

                    x = quad->x2 - chopper.hscroll;
                    y = quad->y2 - chopper.vscroll;

                    vpoke(SPRITE_X_L(x), QUAD_ATTR2 + quad->spriteOffset2);  // Attr2
                    VERA.data0 = SPRITE_X_H(x);                              // Attr3
                    VERA.data0 = SPRITE_Y_L(y);                              // Attr4
                    VERA.data0 = SPRITE_Y_H(y);                              // Attr5
                    VERA.data0 = SPRITE_LAYER1 | SPRITE_HORIZ_FLIP;          // Attr6
                }
            }
            else {
                // Turn sprites off
                vpoke(0, QUAD_ATTR6 + quad->spriteOffset1);
                vpoke(0, QUAD_ATTR6 + quad->spriteOffset2);
            }
        }
        else {
            // Turn sprites off
            vpoke(0, QUAD_ATTR6 + quad->spriteOffset1);
            vpoke(0, QUAD_ATTR6 + quad->spriteOffset2);
        }
    }
    for (i = 0; i < dualBeamCount; i++) {
        struct dual_beam_t *dual = &dualBeams[i];
        if (dual->active) {
#if defined(DEBUG_BEAMS)
            debugDual(dual,yy++);
#endif            
            moveDualBeam(dual);
            if ((chopper.hscroll <= dual->x && (chopper.hscroll + 320) >= dual->x) &&
                (chopper.vscroll <= dual->y && (chopper.vscroll + 240) >= dual->y)) {
                uint16_t x;
                uint16_t y;

                if (dual->delay) {
                    // Turn sprite off
                    vpoke(0, DUAL_ATTR6 + dual->spriteOffset);
                }
                else {

                    // Calculate beam x y positions relative to hscroll/vscroll
                    x = dual->x - chopper.hscroll;
                    y = dual->y - chopper.vscroll;

                    vpoke(SPRITE_X_L(x), DUAL_ATTR2 + dual->spriteOffset);  // Attr2
                    VERA.data0 = SPRITE_X_H(x);                             // Attr3
                    VERA.data0 = SPRITE_Y_L(y);                             // Attr4
                    VERA.data0 = SPRITE_Y_H(y);                             // Attr5
                    VERA.data0 = SPRITE_LAYER1;                             // Attr6
                }
            }
            else {
                // Turn sprite off
                vpoke(0, DUAL_ATTR6 + dual->spriteOffset);
            }
        }
        else {
            // Turn sprite off
            vpoke(0, DUAL_ATTR6 + dual->spriteOffset);
        }
    }
}