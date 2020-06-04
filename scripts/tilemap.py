#!/usr/bin/env python3
#
# Parse a Tiled .tmx file and generate a tilemap metadata file 
# (just containing the tile id for each cell)
#
# Depends on the `tmx` library for Python (https://pythonhosted.org/tmx/)
import argparse
import sys
import tmx

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
    description='Converts .txt file to .bin file')
parser.add_argument('--input', help='.json filename')
parser.add_argument('--output', help='.bin filename')
args = parser.parse_args()

tilemap = None
try:
    tilemap = tmx.TileMap.load(args.input)

except:
    print("Error reading tilemap file")
    sys.exit(1)

with open(args.output,"wb") as out:
    # Handle 2-byte header
    header = bytearray([0x00,0xa0])
    out.write(bytearray(header))

    width = tilemap.width
    height = tilemap.height
    print("Tilemap is %d x %x" % (width,height))
    if len(tilemap.layers) != 1:
        print("Unexpected number of layers")
        sys.exit(1)

    layer = tilemap.layers[0]
    tilecount = len(layer.tiles)

    if tilecount != width * height:
        print("Unexpected number of tiles in layer 0")
        sys.exit(1)

    for tile in layer.tiles:
        # Actual tile id is off by 1
        id = tile.gid - 1
        tile_id = [id & 0xff ]
        out.write(bytearray(tile_id))