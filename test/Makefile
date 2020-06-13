.PHONY: all clean

LIBRARIES := -L ../libX16/lib -L ../libX16Test/lib

INCLUDES := -I ../libX16/include -I ../libX16Test/include -I ../src

SRC = \
	../src/chopper.c ../src/loader.c ../src/keys.c ../src/screen.c ../src/collision.c ../src/turret.c

test_chopper: test_chopper.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -DDEBUG_CHOPPER=1 -Oi -o test_chopper.prg test_chopper.c $(SRC) libX16.lib cx16.lib

test_sprites: test_sprites.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_sprites.prg test_sprites.c $(SRC) libX16.lib cx16.lib

test_tiles: test_tiles.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_tiles.prg test_tiles.c $(SRC) libX16.lib cx16.lib

test_tilemap: test_tilemap.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_tilemap.prg test_tilemap.c $(SRC) libX16.lib cx16.lib

test_turrets: test_turrets.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -Oi -o test_turrets.prg test_turrets.c $(SRC) libX16.lib cx16.lib	

test_collision: test_collision.c $(SRC)
	cl65 -t cx16 $(INCLUDES) $(LIBRARIES) -DUNIT_TEST=1 -Oi -o test_collision.prg test_collision.c $(SRC) libX16.lib libX16Test.lib cx16.lib

# Run collision unit tests in the emulator headless via x16Test framework
run_tests: test_collision
	../libX16Test/bin/runTest.py --prg test_collision.prg --xvfb

clean:
	rm -f *.prg *.o

all: bins test_chopper

bins: ../assets/ChopperRaidPalette.gpl ../assets/bitmap_palette.gpl ../assets/chopper_right.png ../assets/chopper_center.png ../assets/chopper_tilt.png ../assets/chopper_explosion.png ../assets/tileset.png
	rm -f ../assets/chopper.png
	montage -background none -geometry 192x32+0+0 ../assets/chopper_center.png ../assets/chopper_right.png ../assets/chopper_tilt.png ../assets/chopper_explosion.png ../assets/chopper.png 
	rm -f chopper.av
	aloevera create project chopper.av
	aloevera -p chopper.av palette import chopper_palette ../assets/ChopperRaidPalette.gpl
	aloevera -p chopper.av palette import bitmap_palette ../assets/bitmap_palette.gpl
	aloevera -p chopper.av imageset import chopper_imageset 64 32 ../assets/chopper.png
	aloevera -p chopper.av imageset format chopper_imageset chopper_palette 4
	aloevera -p chopper.av sprite init chopper_sprite chopper_imageset
	aloevera -p chopper.av asm -f bin . select chopper_palette PALETTE.BIN
	aloevera -p chopper.av asm -f bin . select chopper_sprite SPRITES.BIN
	aloevera -p chopper.av imageset import chopper_tile_imageset 8 8 ../assets/tileset.png
	aloevera -p chopper.av imageset format chopper_tile_imageset chopper_palette 4
	aloevera -p chopper.av asm -f bin . select chopper_tile_imageset TILES.BIN
	aloevera -p chopper.av imageset import chopper_tile_bitmap 8 8 ../assets/tileset.png
	aloevera -p chopper.av imageset format chopper_tile_bitmap bitmap_palette 1
	aloevera -p chopper.av asm -f bin . select chopper_tile_bitmap META.BIN
	aloevera -p chopper.av tilemap init chopper_tilemap chopper_tile_imageset 128 64 tile_4bpp
	aloevera -p chopper.av tilemap load chopper_tilemap ../assets/tilemap.png
	aloevera -p chopper.av asm -f bin . select chopper_tilemap TILEMAP.BIN
	aloevera -p chopper.av imageset import horiz_turret_imageset 8 16 ../assets/horiz_turret.png
	aloevera -p chopper.av imageset format horiz_turret_imageset chopper_palette 4
	aloevera -p chopper.av sprite init hturret_sprite horiz_turret_imageset
	aloevera -p chopper.av asm -f bin . select hturret_sprite HTURRET.BIN
	aloevera -p chopper.av imageset import vert_turret_imageset 16 8 ../assets/vert_turret.png
	aloevera -p chopper.av imageset format vert_turret_imageset chopper_palette 4
	aloevera -p chopper.av sprite init vturret_sprite vert_turret_imageset
	aloevera -p chopper.av asm -f bin . select vturret_sprite VTURRET.BIN

	# Convert Tiled tilemap into MAPMETA.BIN for bank 1
	../scripts/tilemap.py --input ../assets/tilemap.tmx --output MAPMETA.BIN
	# Convert chopper bitmap png into bitmap appended to META.BIN
	../scripts/bitmap.py --input ../assets/chopper_bitmap.png --output META.BIN
	