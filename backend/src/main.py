""" preprocessing.py
App init functions

Functions:
    main()

Authors:
    Brendan Grau <https://github.com/Victoriam7>
    Keenan Miller <https://github.com/keenanm500>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import sys
import argparse

from pipeline import processing_pipeline
from packet import genpacket, gentarget
from parser import Parser


# Usage help messages
TARGET_TYPE_HELP = 'The color target reference data; defaults to NGT'
TOP_LEFT_X_HELP = 'Pixel value of the top-left of the upright color target from the left end of the color-target image'
BOTTOM_RIGHT_X_HELP = 'Pixel value of the bottom-right of the upright color target from the left end of the color-target image'
TOP_LEFT_Y_HELP = 'Pixel value of the top-left of the upright color target from the top end of the color-target image'
BOTTOM_RIGHT_Y_HELP = 'Pixel value of the bottom-right of the upright color target from the top end of the color-target image'
WHITE_COL_HELP = 'Column of selected white patch from left of target'
WHITE_ROW_HELP = 'Row of selected white patch from top of target'
IMAGES_HELP = '''Images should be added in this order:
        Target A
        Target B
        Flat Field A
        Flat Field B
        Dark Field A
        Dark Field B
        Subject A (Optional)
        Subject B (OptionaL)
        Additional Images... (A and B) '''
OUTPUT_HELP = 'Directory to use for image output instead of current working directory'


def main():
    """ App entry point """
    parser = Parser(formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('-t', '--target', choices=['NGT', 'APT', 'CCSG', 'CC'],
                        default='NGT', help=TARGET_TYPE_HELP)

    # Top left & bottom right of (NGT at the moment) target (pixels)
    parser.add_argument('top_left_x', help=TOP_LEFT_X_HELP)
    parser.add_argument('bottom_right_x', help=BOTTOM_RIGHT_X_HELP)
    parser.add_argument('top_left_y', help=TOP_LEFT_Y_HELP)
    parser.add_argument('bottom_right_y', help=BOTTOM_RIGHT_Y_HELP)

    # Location of white square on target image
    parser.add_argument('white_col', help=WHITE_COL_HELP)
    parser.add_argument('white_row', help=WHITE_ROW_HELP)

    parser.add_argument('images', nargs='+', help=IMAGES_HELP)

    # Optional arg to override default output directory (current working)
    parser.add_argument('output', required=False, help=OUTPUT_HELP)

    args = parser.parse_args()

    if len(args.images) < 6:
        parser.print_help()
        sys.exit(1)

    # Gather target coords and white square
    top_left = (int(args.top_left_x), int(args.top_left_y))
    bottom_right = (int(args.bottom_right_x), int(args.bottom_right_y))

    target = gentarget(top_left, bottom_right, (int(args.white_row),
                                                int(args.white_col)))

    # Setup packet
    packet = build_packet(args.images, target)

    # Begin pipeline
    processing_pipeline(packet)


def build_packet(images, target):
    """ Create packet
    [in] image  : image files
    [in] target : target grid
    [out] packet
    """
    packet = genpacket(images, target)
    return packet


if __name__ == "__main__":
    main()
