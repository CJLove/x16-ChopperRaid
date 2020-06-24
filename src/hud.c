#include "hud.h"
#include "screen.h"
#include <cx16.h>
#include <stdio.h>
#include <vera.h>

#define TILE_BLANK 32
#define TILE_BACK 95
#define TILE_KEY 67
#define TILE_JAMMER 66
#define TILE_FUEL_FULL 84
#define TILE_AMMO_FULL 96
#define TILE_TOKEN_MIN 68
#define TILE_TOKEN_MAX 83
#define TOKEN_OFFSET 4

// Fuel or Ammo increments
#define INCREMENTS 32

// Tile animation for first 3 digits of fuel
static uint8_t fuelTiles[] = {91, 90, 89, 88, 87, 86, 85, 84};
// Tile animation for last digit of fuel
static uint8_t fuelEmptyTiles[] = {93, 92, 89, 88, 87, 86, 85, 84};

// Tile animation for first 3 digits of ammo
static uint8_t ammoTiles[] = {103, 102, 101, 100, 99, 98, 97, 96};
// Tile animation for last digit of ammo
static uint8_t ammoEmptyTiles[] = {93, 92, 101, 100, 99, 98, 97, 96};

static uint8_t hudTiles[3][40] = {
    // Top row
    {19,        3, 15, 18, 5,  58, 48, 48, 48, 48, 48,        48, TILE_BACK, TILE_KEY, 58, 48, TILE_BACK, TILE_JAMMER, 58, 48,
     TILE_BACK, 6, 21, 5,  12, 58, 84, 84, 84, 84, TILE_BACK, 1,  13,        13,       15, 58, 96,        96,          96, 96},
    // Second row
    {26,        15,        14,        5,         58,        49,        TILE_BACK, 12,        9,         22,
     5,         19,        58,        48,        48,        TILE_BACK, TILE_BACK, TILE_BACK, 21,        14,
     9,         20,        19,        58,        TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK,
     TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK, TILE_BACK},
    // Third row (separator)
    {
        94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
        94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
    }};

void displayStatic(void)
{
    uint8_t x = 0;
    uint8_t y = 0;

    setBase(LAYER1_MAP_BASE);
    for (y = 0; y < 3; y++) {
        for (x = 0; x < SCREEN_WIDTH; x++) {
            setTile(x, y, hudTiles[y][x], 0);
        }
    }
}

void displayScore(uint32_t score)
{
    uint8_t i = 0;
    char buf[7];

    setBase(LAYER1_MAP_BASE);
    sprintf(buf, "%06lu", score);
    for (i = 0; i < 6; i++) {
        setTile(6 + i, 0, buf[i], 0);
    }
}

void displayZone(uint8_t zone)
{
    char buf[2];
    sprintf(buf, "%d", zone);

    setBase(LAYER1_MAP_BASE);
    setTile(5, 1, buf[0], 0);
}

void displayLives(uint8_t lives)
{
    char buf[3];
    uint8_t i = 0;

    setBase(LAYER1_MAP_BASE);
    sprintf(buf, "%02u", lives);
    for (i = 0; i < 2; i++) {
        setTile(13 + i, 1, buf[i], 0);
    }
}

void displayKeys(uint8_t keys)
{
    char buf[2];

    setBase(LAYER1_MAP_BASE);
    sprintf(buf, "%d", keys);
    setTile(15, 0, buf[0], 0);
}

void displayJammers(uint8_t jammers)
{
    char buf[2];

    setBase(LAYER1_MAP_BASE);
    sprintf(buf, "%d", jammers);
    setTile(19, 0, buf[0], 0);
}

void displayFuel(uint16_t fuel)
{
    uint16_t scaled = (fuel / 2048)-1;
    fuel /= 2048;
    setBase(LAYER1_MAP_BASE);

    if (fuel > 32) {
        setTile(26, 0, TILE_FUEL_FULL, 0);
        setTile(27, 0, TILE_FUEL_FULL, 0);
        setTile(28, 0, TILE_FUEL_FULL, 0);
        setTile(29, 0, TILE_FUEL_FULL, 0);
    }
    else if (fuel > 24) {
        setTile(26, 0, fuelTiles[scaled % 8], 0);
        setTile(27, 0, TILE_FUEL_FULL, 0);
        setTile(28, 0, TILE_FUEL_FULL, 0);
        setTile(29, 0, TILE_FUEL_FULL, 0);
    }
    else if (fuel > 16) {
        // Digit 1 blank
        setTile(26, 0, TILE_BACK, 0);
        setTile(27, 0, fuelTiles[scaled % 8], 0);
        setTile(28, 0, TILE_FUEL_FULL, 0);
        setTile(29, 0, TILE_FUEL_FULL, 0);
    }
    else if (fuel > 8) {
        // Digits 1 and 2 blank
        setTile(26, 0, TILE_BACK, 0);
        setTile(27, 0, TILE_BACK, 0);
        setTile(28, 0, fuelTiles[scaled % 8], 0);
        setTile(29, 0, TILE_FUEL_FULL, 0);
    }
    else if (fuel > 0) {
        // Digits 1-3 blank
        setTile(26, 0, TILE_BACK, 0);
        setTile(27, 0, TILE_BACK, 0);
        setTile(28, 0, TILE_BACK, 0);
        setTile(29, 0, fuelEmptyTiles[scaled % 8], 0);
    }
    else {
        // All digits blank
        setTile(26, 0, TILE_BACK, 0);
        setTile(27, 0, TILE_BACK, 0);
        setTile(28, 0, TILE_BACK, 0);
        setTile(29, 0, TILE_BACK, 0);
    }
}

void displayAmmo(uint16_t ammo) 
{
    // TODO: Convert ammo to scaled value (0..31)
    int16_t scaled = ammo - 1;
    setBase(LAYER1_MAP_BASE);

    if (ammo > 32) {
        setTile(36, 0, TILE_AMMO_FULL, 0);
        setTile(37, 0, TILE_AMMO_FULL, 0);
        setTile(38, 0, TILE_AMMO_FULL, 0);
        setTile(39, 0, TILE_AMMO_FULL, 0);
    }
    else if (ammo > 24) {
        setTile(36, 0, ammoTiles[scaled % 8], 0);
        setTile(37, 0, TILE_AMMO_FULL, 0);
        setTile(38, 0, TILE_AMMO_FULL, 0);
        setTile(39, 0, TILE_AMMO_FULL, 0);
    }
    else if (ammo > 16) {
        // Digit 1 blank
        setTile(36, 0, TILE_BACK, 0);
        setTile(37, 0, ammoTiles[scaled % 8], 0);
        setTile(38, 0, TILE_AMMO_FULL, 0);
        setTile(39, 0, TILE_AMMO_FULL, 0);
    }
    else if (ammo > 8) {
        // Digits 1 and 2 blank
        setTile(36, 0, TILE_BACK, 0);
        setTile(37, 0, TILE_BACK, 0);
        setTile(38, 0, ammoTiles[scaled % 8], 0);
        setTile(39, 0, TILE_AMMO_FULL, 0);
    }
    else if (ammo > 0) {
        // Digits 1-3 blank
        setTile(36, 0, TILE_BACK, 0);
        setTile(37, 0, TILE_BACK, 0);
        setTile(38, 0, TILE_BACK, 0);
        setTile(39, 0, ammoEmptyTiles[scaled % 8], 0);
    }
    else {
        // All digits blank
        setTile(36, 0, TILE_BACK, 0);
        setTile(37, 0, TILE_BACK, 0);
        setTile(38, 0, TILE_BACK, 0);
        setTile(39, 0, TILE_BACK, 0);
    }
}

void displayToken(uint8_t token)
{
    uint8_t hudToken = token - TOKEN_OFFSET;
    setBase(LAYER1_MAP_BASE);
    setTile(24 + hudToken, 1, hudToken + TILE_TOKEN_MIN, 0);
}

void clearToken(uint8_t token)
{
    uint8_t hudToken = token - TOKEN_OFFSET;
    setBase(LAYER1_MAP_BASE);
    setTile(24 + hudToken, 1, TILE_BLANK, 0);
}

void clearTokens(void);