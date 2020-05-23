.PHONY: all clean

LIBRARIES := -L ../libX16/lib -L ../libX16Test/lib

INCLUDES := -I ../libX16/include -I ../libX16Test/include -I ../src

SRC = \
	../src/chopper.c ../src/loader.c ../src/keys.c ../src/screen.c

test_chopper: test_chopper.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_chopper.prg test_chopper.c $(SRC) libX16.lib cx16.lib

test_sprites: test_sprites.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_sprites.prg test_sprites.c $(SRC) libX16.lib cx16.lib

test_tiles: test_tiles.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_tiles.prg test_tiles.c $(SRC) libX16.lib cx16.lib

test_tilemap: test_tilemap.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_tilemap.prg test_tilemap.c $(SRC) libX16.lib cx16.lib

clean:
	rm -f *.prg *.o

all: bins test_chopper

bins: ../assets/palette.png ../assets/chopper_right.png ../assets/chopper_center.png ../assets/chopper_tilt.png ../assets/tileset.png
	rm -f ../assets/chopper.png
	montage -background none -geometry 192x32+0+0 ../assets/chopper_center.png ../assets/chopper_right.png ../assets/chopper_tilt.png ../assets/chopper.png
	rm -f chopper.av
	aloevera create project chopper.av
	aloevera -p chopper.av palette import palette ../assets/palette.png
	aloevera -p chopper.av imageset import chopper_imageset 64 32 ../assets/chopper.png
	aloevera -p chopper.av imageset format chopper_imageset palette 4
	aloevera -p chopper.av sprite init chopper_sprite chopper_imageset
	aloevera -p chopper.av asm -f bin . select palette PALETTE.BIN
	aloevera -p chopper.av asm -f bin . select chopper_sprite SPRITES.BIN
	aloevera -p chopper.av imageset import chopper_tile_imageset 8 8 ../assets/tileset.png
	aloevera -p chopper.av imageset format chopper_tile_imageset palette 4
	aloevera -p chopper.av asm -f bin . select chopper_tile_imageset TILES.BIN
	aloevera -p chopper.av tilemap init chopper_tilemap chopper_tile_imageset 128 64 tile_4bpp
	aloevera -p chopper.av tilemap load chopper_tilemap ../assets/tilemap.png
	aloevera -p chopper.av asm -f bin . select chopper_tilemap TILEMAP.BIN