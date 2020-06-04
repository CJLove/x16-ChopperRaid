#!/usr/bin/env python3
#
# Parse a Tiled .tmx file and generate a tilemap metadata file 
# (just containing the tile id for each cell)
#
# Depends on the `pil` library for Python (https://pillow.readthedocs.io/en/stable/reference/Image.html)
import argparse
import sys
from PIL import Image

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
    description='Converts .txt file to .bin file')
parser.add_argument('--input', help='.json filename')
parser.add_argument('--output', help='.bin filename')
parser.add_argument('--pad', help='number of bytes to pad')
args = parser.parse_args()

pad=0
try:
    pad = int(args.pad)
except:
    pass

img = None
try:
    img = Image.open(args.input)

except:
    print("Error reading png (bitmap) file")
    sys.exit(1)

with open(args.output,"ab") as out:
    width = img.width
    height = img.height
    print("Image is %d x %d" % (width,height))

    for i in range(pad):
        pad = [0];
        out.write(bytearray(pad))

    for y in range(height):
        bits = 0
        for x in range(width):
            pixel = 1
            png_pixel = img.getpixel((x,y))
            if png_pixel == (0,0,0,0):
                pixel = 0
            bit_num = x % 8
            if pixel:
                bits |= (1 << (7 - bit_num))
            #print ("%d" %pixel, end="")

            if bit_num == 7:
                #print("bits=0x%x" % bits)
                bits_arr = [ bits & 0xff ]
                out.write(bytearray(bits_arr))
                #print(" 0x%x" % bits)
                bits = 0
