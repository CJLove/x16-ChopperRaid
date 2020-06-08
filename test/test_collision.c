#include "chopper.h"
#include "keys.h"
#include "screen.h"
#include "collision.h"
#include "load.h"
#include "x16test.h"
#include <conio.h>
#include <cx16.h>
#include <joystick.h>
#include <stdint.h>
#include <stdio.h>

extern int loadFiles();
extern int loadTilemap();

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

// Tilemap dimensions
#define MAP_WIDTH 128
#define MAP_HEIGHT 64

// Representation of tilemap metadata as stored in RAM bank 1
struct TilemapMetadata_t {
    uint8_t tiles[MAP_HEIGHT][MAP_WIDTH];
};

static struct TilemapMetadata_t *tileMeta = (struct TilemapMetadata_t*)0xa000;

void dumpTilemap(uint8_t x, uint8_t y)
{
    VIA1.pra = 1;
    printf("Tilemap x=%d y=%d = %d\n",x,y,tileMeta->tiles[y][x]);
}

void dumpTile(uint8_t tid)
{
    printf("Tile %d: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",tid,
        meta->tiles[tid].rows[0], meta->tiles[tid].rows[1], meta->tiles[tid].rows[2], meta->tiles[tid].rows[3],
        meta->tiles[tid].rows[4], meta->tiles[tid].rows[5], meta->tiles[tid].rows[6], meta->tiles[tid].rows[7]);
}

void dumpSprite(uint8_t sid)
{
    uint8_t row = 0;
    printf("Sprite %d\n",sid);
    for (row = 0; row < 24; row++) {
        printf("  %02d: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",row,
            meta->sprites[sid].pixels[row][0],meta->sprites[sid].pixels[row][1],meta->sprites[sid].pixels[row][2],
            meta->sprites[sid].pixels[row][3],meta->sprites[sid].pixels[row][4],meta->sprites[sid].pixels[row][5],
            meta->sprites[sid].pixels[row][6]
        );
    }
}

void clearTiles() {
    int x = 0;
    int y = 0;
    for (y = 0; y < 4; y++) {
        for (x = 0; x < 7; x++) {
            tiles[y][x] = 32;
        }
    }
}

int testCoarseSpecial()
{
    int result = 0;
    uint8_t x = 0;
    TEST_INIT()

    chopper.sequence = CHOPPER_FULL_LEFT;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    clearTiles();
    for (x = 0; x < 7; x++) {
        tiles[3][x] = 68;
    }
    tiles[3][3] = 119;  // "key"
    chopper.partialY = 4;

    result = checkCoarseCollision();
    EXPECT_EQ(0,result);

    TEST_COMPLETE()
}

int testLandFarLeft()
{
    int result = 0;
    uint8_t x = 0;
    TEST_INIT()

    chopper.sequence = CHOPPER_FULL_LEFT;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    // PartialY > 0 
    clearTiles();
    chopper.partialY = 1;
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 80;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 96;
    chopper.partialY = 7;
    result = okToLand();
    EXPECT_EQ(0,result);

    // PartialY = 0, full range
    chopper.partialY = 0;
    for (x = 0; x < 8; x++) {
        chopper.partialX = x;
        result = okToLand();
        EXPECT_EQ(1,result);
    }

    clearTiles();
    chopper.partialX = 0;
    tiles[2][0] = tiles[2][1] = tiles[2][2] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 3;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 4;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][3] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 7;
    result = okToLand();
    EXPECT_EQ(1,result);        

    TEST_COMPLETE()
}

int testLandLeft()
{
    int result = 0;
    uint8_t x = 0;
    TEST_INIT()

    chopper.sequence = CHOPPER_LEFT;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    // PartialY > 0 
    clearTiles();
    chopper.partialY = 1;
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 80;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 96;
    chopper.partialY = 7;
    result = okToLand();
    EXPECT_EQ(0,result);

    // PartialY = 0, full range
    chopper.partialY = 0;
    for (x = 0; x < 8; x++) {
        chopper.partialX = x;
        result = okToLand();
        EXPECT_EQ(1,result);
    }

    clearTiles();
    chopper.partialX = 0;
    tiles[2][0] = tiles[2][1] = tiles[2][2] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 3;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 4;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][3] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 7;
    result = okToLand();
    EXPECT_EQ(1,result);        

    TEST_COMPLETE()
}

int testLandCenter()
{
    int result = 0;
    uint8_t x = 0;
    TEST_INIT()

    chopper.sequence = CHOPPER_CENTER;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    // PartialY > 0 
    clearTiles();
    chopper.partialY = 1;
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 80;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 96;
    chopper.partialY = 7;
    result = okToLand();
    EXPECT_EQ(0,result);

    // PartialY = 0, full range
    chopper.partialY = 0;
    for (x = 0; x < 8; x++) {
        chopper.partialX = x;
        result = okToLand();
        EXPECT_EQ(1,result);
    }

    clearTiles();
    chopper.partialX = 0;
    tiles[2][2] = tiles[2][3] = 96;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 5;
    result = okToLand();
    EXPECT_EQ(1,result);    
    chopper.partialX = 6;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][4] = 96;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 7;
    result = okToLand();
    EXPECT_EQ(1,result);    

    TEST_COMPLETE()
}

int testLandRight()
{
    int result = 0;
    uint8_t x = 0;
    TEST_INIT()

    chopper.sequence = CHOPPER_RIGHT;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    // PartialY > 0 
    clearTiles();
    chopper.partialY = 1;
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 80;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 96;
    chopper.partialY = 7;
    result = okToLand();
    EXPECT_EQ(0,result);

    // PartialY = 0, full range
    chopper.partialY = 0;
    for (x = 0; x < 8; x++) {
        chopper.partialX = x;
        result = okToLand();
        EXPECT_EQ(1,result);
    }

    clearTiles();
    chopper.partialX = 0;
    tiles[2][3] = tiles[2][4] = tiles[2][5] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 4;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 5;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][6] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 7;
    result = okToLand();
    EXPECT_EQ(1,result);        

    TEST_COMPLETE()
}

int testLandFullRight()
{
    int result = 0;
    uint8_t x = 0;
    TEST_INIT()

    chopper.sequence = CHOPPER_FULL_RIGHT;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    // PartialY > 0 
    clearTiles();
    chopper.partialY = 1;
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 80;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][0] = tiles[2][1] = tiles[2][2] = tiles[2][3] = tiles[2][4] = tiles[2][5] = tiles[2][6] = 96;
    chopper.partialY = 7;
    result = okToLand();
    EXPECT_EQ(0,result);

    // PartialY = 0, full range
    chopper.partialY = 0;
    for (x = 0; x < 8; x++) {
        chopper.partialX = x;
        result = okToLand();
        EXPECT_EQ(1,result);
    }

    clearTiles();
    chopper.partialX = 0;
    tiles[2][3] = tiles[2][4] = tiles[2][5] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 4;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 5;
    result = okToLand();
    EXPECT_EQ(0,result);
    tiles[2][6] = 80;
    result = okToLand();
    EXPECT_EQ(1,result);
    chopper.partialX = 7;
    result = okToLand();
    EXPECT_EQ(1,result);        

    TEST_COMPLETE()
}

int testChopperFarLeft()
{
    int result = 0;

    TEST_INIT()

    chopper.sequence = CHOPPER_FULL_LEFT;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    // yoffset = 0
    clearTiles();
    tiles[0][0] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[0][1] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[0][1] = 64;   // Expect collision for x=1,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[0][1] = 71;
    tiles[0][2] = 72;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[0][1] = 71;
    tiles[0][2] = 69;   // Expect collision for x=2,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[0][2] = 72;
    tiles[0][3] = 72;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[0][3] = 71;   // Expect collision for x=3,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[0][3] = 72;
    tiles[0][4] = 64;   // Expect collision for x=4,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[0][4] = 32;
    tiles[0][5] = 64;   // Expect collision for x=5,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[0][5] = 32;
    tiles[1][0] = 64;   // Expect collision for x=0,y=1
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[1][0] = 32;
    tiles[1][4] = 68;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[1][4] = 64;   // Expect collision for x=4,y=1
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[1][4] = 32;
    tiles[1][5] = 64;   // Expect collision for x=5,y=1
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[1][5] = 32;
    tiles[2][0] = 93;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[2][0] = 64;   // Expect collision for x=0,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[2][0] = 32;
    tiles[2][1] = 67;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[2][1] = 64;   // Expect collision for x=1,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[2][1] = 32;
    tiles[2][2] = 66;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[2][2] = 64;   // Expect collision for x=2,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    // Shift chopper partialX by 7 (pixels)
    chopper.partialX = 7;
    tiles[2][2] = 32;
    tiles[0][6] = 64;   // Expect collision for x=6,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    // Shift chopper partialY by 1 (pixel)
    clearTiles();
    tiles[0][1] = 64;

    chopper.partialX = 7;
    chopper.partialY = 1;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    // Shift chopper partialY by 4 (pixels)
    clearTiles();
    tiles[0][3] = 71;
    chopper.partialX = 7;
    chopper.partialY = 4;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[0][3] = 64; // Expect collision for x=3,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    tiles[2][3] = 67; // Expect collision for x=3,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    // Shift chopper partialY by 7 pixels
    clearTiles();
    chopper.partialX = 7;
    chopper.partialY = 7;
    tiles[3][1] = 67;
    result = checkFineCollision();
    EXPECT_EQ(0,result);
    tiles[3][1] = 66;   // Expect collision for x=1,y=3
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    TEST_COMPLETE()
}

int testChopperLeft()
{
    int result = 0;
    TEST_INIT()

    chopper.sequence = CHOPPER_LEFT;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;

    clearTiles();
    tiles[0][0] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[0][0] = 70;   // Expect collision for x=0,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result); 

    clearTiles();
    tiles[0][1] = 70;  // Expect collision for x=1,y=0 
    result = checkFineCollision();
    EXPECT_EQ(1,result); 

    clearTiles();
    tiles[0][2] = 70;  
    result = checkFineCollision();
    EXPECT_EQ(0,result); 

    tiles[0][2] = 69;   // Expect collision for x=2,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result); 

    clearTiles();
    tiles[0][3] = 70;   // Expect collision for x=3,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);  

    clearTiles();
    tiles[0][4] = 70;   // Expect collision for x=4,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result); 

    clearTiles();
    tiles[0][5] = 70;
    result = checkFineCollision();
    EXPECT_EQ(0,result); 

    tiles[0][5] = 64;   // Expect collision for x=5,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);    

    // xOffset set to 4 pixels
    chopper.partialX = 4;
    clearTiles();
    tiles[1][0] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result); 

    tiles[2][0] = 64;    
    result = checkFineCollision();
    EXPECT_EQ(0,result); 

    tiles[1][6] = 64;   // Expect collision for x=6,y=1
    result = checkFineCollision();
    EXPECT_EQ(1,result);    

    clearTiles();
    tiles[2][2] = 66;   // Expect collision for x=2,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result); 

    clearTiles();
    tiles[2][3] = 66;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  

    tiles[2][3] = 65;   // Expect collision for x=3,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);            

    // yOffset set to 4
    chopper.partialY = 4;
    clearTiles();
    tiles[0][0] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  
    tiles[0][1] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  
    tiles[0][2] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  
    tiles[0][3] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  
    tiles[0][4] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  

    clearTiles();
    tiles[2][1] = 91;   // Expect collision for x=1,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);  

    clearTiles();
    tiles[2][2] = 91;   // Expect collision for x=2,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);                        

    clearTiles();
    tiles[2][3] = 80;   // Expect collision for x=3,y=2
    result = checkFineCollision();
    EXPECT_EQ(1,result);   

    clearTiles();
    tiles[2][4] = 80;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  

    // yOffset set to 7
    clearTiles();    
    chopper.partialY = 7;
    chopper.partialX = 0;
    tiles[3][1] = 67;
    result = checkFineCollision();
    EXPECT_EQ(0,result);  
    tiles[3][1] = 66;   // Expect collision for x=1,y=3
    result = checkFineCollision();
    EXPECT_EQ(1,result);         

    TEST_COMPLETE()
}

int testChopperCenter()
{
    int result = 0;

    TEST_INIT()

    chopper.sequence = CHOPPER_CENTER;
    chopper.x = 0;
    chopper.y = 0;
    chopper.tx = 0;
    chopper.ty = 0;
    chopper.partialX = 0;
    chopper.partialY = 0;  

    clearTiles();
    tiles[0][0] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);
    tiles[0][1] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);
    tiles[0][2] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);
    tiles[0][3] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);
    tiles[0][4] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);
    tiles[0][5] = 71;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[0][0] = 69;   // Expect collision for x=0,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);            

    clearTiles();
    tiles[1][0] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[1][5] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);       

    tiles[2][1] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    tiles[2][4] = 64;
    result = checkFineCollision();
    EXPECT_EQ(0,result);

    // Setting xOffset to 7
    chopper.partialX = 7; 
    clearTiles();
    tiles[0][6] = 64;   // Expect collision for x=6,y=0
    result = checkFineCollision();
    EXPECT_EQ(1,result);  

    clearTiles();
    tiles[1][5] = 64;   // Expect collision for x=6,y=1
    result = checkFineCollision();
    EXPECT_EQ(1,result);

    // Setting yOffset to 7
    chopper.partialY = 7;
    clearTiles();


    TEST_COMPLETE()
}

int main()
{
    int i = 0;
    int result = 0;
    int x = 0;
    int y = 0;

    result = load_bank_host("meta.bin",2);
    if (result) 
        printf("  loaded tile/sprite metadata\n");
    else {
        printf("  failed to load tile/sprite metadata\n");
        return 0;
    }


    // dumpTilemap(0,20);
    // dumpTilemap(1,20);
    // dumpTilemap(2,20);
    // return 0;

    // Bank 2
    VIA1.pra = 2;

    // Debugging: dump a specific tile's bitmap
    //dumpTile(70);
    // Debugging: dump a specific sprite's bitmap
    //dumpSprite(8);

    registerTestModule(testCoarseSpecial,"Coarse special tiles");
#if 0
    registerTestModule(testLandFarLeft,"Land full left");
    registerTestModule(testLandLeft,"Land left");
    registerTestModule(testLandCenter,"Land center");
    registerTestModule(testLandRight,"Land right");
    registerTestModule(testLandFullRight,"Land full right");
    registerTestModule(testChopperFarLeft,"Chopper full left");
    registerTestModule(testChopperLeft,"Chopper left");
    registerTestModule(testChopperCenter,"Chopper center");
#endif
    return x16testmain(TEST_EXIT);
}