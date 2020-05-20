#include <stdint.h>

#define LAYER0_OFFSET  0x8000
#define LAYER1_OFFSET  0x0000

extern int screenConfig(void);

extern int screenReset(void);

void setBase(uint32_t base);

void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset);
