#include "chopper.h"
#include "screen.h"
#include "collision.h"
#include <stdio.h>

struct TileMetadata_t {
    uint8_t rows[8];
};

struct SpriteMetadata_t {
    uint8_t pixels[24][7];
};

struct Metadata_t {
    // Bitmap representation of tilesets
    struct TileMetadata_t tiles[128];

    // Bitmap representation of chopper sprites
    struct SpriteMetadata_t sprites[40];
};

static struct Metadata_t *meta = (struct Metadata_t *) 0xa000;

#define TILE_BLANK 32
#define TILE_PAD1 80
#define TILE_PAD2 96
#define TILE_KEY  119
#define TILE_TOKEN_MAX  127

int okToLand()
{
    uint8_t pad = 0;
    uint8_t startX = 0;
    uint8_t endX = 0;

    if (chopper.partialY != 0) {
        return 0;
    }

    switch (chopper.sequence) {
    case CHOPPER_FULL_LEFT:
        startX = 0 + (chopper.partialX >= 4);
        endX = 2 + (chopper.partialX >= 4);
        break;
    case CHOPPER_LEFT:
        startX = 0 + (chopper.partialX >= 4);
        endX = 2 + (chopper.partialX >= 4);
        break;
    case CHOPPER_CENTER:
        startX = 2 + (chopper.partialX >= 6);
        endX = 3 + (chopper.partialX >= 6);
        break;
    case CHOPPER_RIGHT:
        startX = 3 + (chopper.partialX >= 5);
        endX = 5 + (chopper.partialX >= 5);
        break;
    case CHOPPER_FULL_RIGHT:
        startX = 3 + (chopper.partialX >= 5);
        endX = 5 + (chopper.partialX >= 5);
        break;
    default:
        return 0;
    }
    pad = tiles[2][startX];
    if (pad == TILE_PAD1 || pad == TILE_PAD2) {
        int ok = 1;
        int i = 1;
        for (i = startX; i <= endX; i++) {
            ok &= (tiles[2][i] == pad);
            if (!ok) return 0;
        }
        return 1;
    }
    return 0;
}

void handleSpecialTile(uint8_t tx, uint8_t ty, uint8_t tile)
{
    // Display the special tile in the heads up display
    setBase(LAYER1_MAP_BASE);
    setTile(30+(tile - TILE_KEY),0,tile,0);

    // Remove the tile from the tilemap and meta tilemap
    setBase(LAYER0_MAP_BASE);
    setTile(tx,ty,TILE_BLANK,0);
    setMetaTile(tx,ty,TILE_BLANK);


}

int checkCoarseCollision()
{
    int i = 0, j = 0;
    // Y limit is 3 tiles unless partialY > 4, then it is 4 tiles
    int yLimit = 3 + (chopper.partialY >= 4);
    // X limit is 6 tiles unless partialX > 0, then it is 7 tiles
    int xlimit = 6 + (chopper.partialX > 0);
#if defined(DEBUG_CHOPPER)                
    setBase(LAYER1_MAP_BASE);
    setTile(38,0,48+yLimit,0);
#endif
    if (yLimit == 4) {
        for (i = 0; i < xlimit; i++) {
            // Handle any "special" tiles in tiles[3][x] which get picked up by the chopper
            // Do this only if chopper.partialY >= 4)            
            uint8_t tile = tiles[3][i];
            if (tile >= TILE_KEY && tile <= TILE_TOKEN_MAX) {
#if !defined(UNIT_TEST)
                handleSpecialTile(chopper.tx + i,chopper.ty + 3,tile);
#endif          
                return 0;
            }
        }
    }
    for (j = 0; j < yLimit; j++) {
        for (i = 0; i < xlimit; i++)
        {
            if (tiles[j][i] != TILE_BLANK) {
                // Return 1 and indicate that "fine" collision detection is needed
#if defined(DEBUG_CHOPPER)                
                setBase(LAYER1_MAP_BASE);
                setTile(31,6,24,0);
#endif                
                return 1;
            }
        }
    }
#if !defined(UNIT_TEST)    
    setBase(LAYER1_MAP_BASE);
    setTile(31,6,TILE_BLANK,0);
    setTile(31,7,TILE_BLANK,0);
#endif    
    return 0;
}

int compareSpriteToTile(uint8_t tileId, uint8_t idx, uint8_t x, uint8_t y, uint8_t yOffset)
{
    struct TileMetadata_t *tile = &meta->tiles[tileId];
    struct SpriteMetadata_t *sprite = &meta->sprites[idx];
    int i = 0;
    
    int8_t spriteY = (y * 8) - yOffset;

    for (i = 0; i < 8; i++) {
        if ((spriteY + i) < 0) {
            continue;
            //printf("Skipping\n");
        }

        //printf("i=%d: Tile = 0x%02x Sprite = 0x%02x\n",i,tile->rows[i],sprite->pixels[spriteY+i][x]);
        if (tile->rows[i] & sprite->pixels[spriteY+i][x]) {
            return 1;
        }
    }
    return 0;
}

int checkFineCollision()
{
    uint8_t idx = 0;
    uint8_t startX = 0;
    uint8_t startY = 0;
    uint8_t xLimit = 6 + (chopper.partialX != 0);
    uint8_t yLimit = 3 + (chopper.partialY > 4);
    uint8_t y = 0;
    uint8_t x = 0;
    // Switch to bank 1 for tile and sprite metadata (bitmaps)
    VIA1.pra = 2;
#if defined(DEBUG_CHOPPER)
    setBase(LAYER1_MAP_BASE);
#endif

    // Determine the chopper bitmap to use
    switch (chopper.sequence) {
    case CHOPPER_FULL_LEFT:
        idx = 0 + chopper.partialX;
        break;
    case CHOPPER_LEFT:
        idx = 8 + chopper.partialX;
        break;
    case CHOPPER_CENTER:
        idx = 16 + chopper.partialX;
        break;
    case CHOPPER_RIGHT:
        idx = 24 + chopper.partialX;
        break;
    case CHOPPER_FULL_RIGHT:
        idx = 32 + chopper.partialX;
        break;
    default:
        // Unable to determine chopper bitmap
#if defined(DEBUG_CHOPPER)        
        setTile(31,7,24,0); // Set "Fine indicator"
#endif        
        return 1; 
    }

    // At this point we have the 3-4 x 6-7 set of tiles matching the chopper position
    // in the tiles[][] array to indicate tile bitmaps to check against.  We also have 
    // the proper chopper bitmap accounting for any "partialX" value (0-8).  We now
    // need to do bitwise comparisons between them, stopping once any bits match.
    for (y = 0; y < yLimit; y++) {
        for (x = 0; x < xLimit; x++) {
            if (compareSpriteToTile(tiles[y][x], idx, x, y, chopper.partialY)) {
#if defined(UNIT_TEST)                
                printf("Collision for x=%d y=%d idx=%d tile=%d\n",x,y,idx,tiles[y][x]);
#endif
#if defined(DEBUG_CHOPPER)
                setTile(31,7,24,0);
#endif
                return 1;
            }
        }
    }
#if defined(DEBUG_CHOPPER)    
    setTile(31,7,32,0);
#endif    
    return 0;
}