#include <stdint.h>


extern int screenConfig(void);

extern int screenReset(void);

void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset);
