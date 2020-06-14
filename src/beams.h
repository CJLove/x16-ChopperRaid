#include <stdint.h>

#define QUAD_BEAM_IMAGE  0x1f200
#define DUAL_VBEAM_IMAGE  0x1f220
#define DUAL_HBEAM_IMAGE  0x1f240

struct quad_beam_t {
    // Sprite coordinates
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    uint16_t spriteOffset1;
    uint16_t spriteOffset2;

    // Are beams currently active for this zone?
    uint8_t active;
    // Maximum number of incremental moves
    uint8_t maxMoves;
    // Current move
    uint8_t curMove;
    // Delay count
    uint8_t delay;

    // Tile dimensions of upper left and upper right
    uint8_t limit1_x;
    uint8_t limit1_y;
    uint8_t limit2_x;
    uint8_t limit2_y;
};

struct dual_beam_t {
    // Sprite coordinates
    uint16_t x;
    uint16_t y;
    uint16_t spriteOffset;

    // Is beam currently active for this zone
    uint8_t active;
    // Orientation (0 = vert)
    uint8_t orientation;
    // Maximum number of incremental moves
    uint8_t maxMoves;
    // Current move
    uint8_t curMove;
    // Delay count
    uint8_t delay;    

    // Tile dimensions of limits
    uint8_t limit1_x;
    uint8_t limit1_y;
    uint8_t limit2_x;
    uint8_t limit2_y;
};


extern int initBeams(void);

extern void updateBeams();