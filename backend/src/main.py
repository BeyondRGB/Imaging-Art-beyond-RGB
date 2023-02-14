""" preprocessing.py
App init functions

Functions:
    main()

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import sys

# Local imports
from pipelines import processing_pipeline
from packet import Packet
from target import Target
import argparse


def main():
    """ App entry point """
    parser = argparse.ArgumentParser(formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('-t', '--target', choices=['NGT', 'APT', 'CCSG', 'CC'], default='NGT', help='The color target reference data')

    """
    Top left & bottom right of (NGT at the moment) target (pixels)
    """
    parser.add_argument('top_left_x', help='Pixel value of the top-left of the color target from the left end of the color-target image')
    parser.add_argument('bottom_right_x', help='Pixel value of the bottom-right of the color target from the left end of the color-target image')
    parser.add_argument('top_left_y', help='Pixel value of the top-left of the color target from the top end of the color-target image')
    parser.add_argument('bottom_right_y', help='Pixel value of the bottom-right of the color target from the top end of the color-target image')

    """
    Location of white square on target image
    """
    parser.add_argument('white_col', help='Column of selected white patch from left of target')
    parser.add_argument('white_row', help='Row of selected white patch from top of target')

    images_help = '''
    Images should be added in this order:
        Target A
        Target B
        Flat Field A
        Flat Field B
        Dark Field A
        Dark Field B
        Subject A (Optional)
        Subject B (OptionaL)
        Additional Images... (A and B)
    '''
    parser.add_argument('images', nargs='+', help=images_help)
    args = parser.parse_args()

    if len(args.images) < 6:
        parser.print_help()
        sys.exit(1)

    """ Setup packet """
    packet = Packet()

    # Gather target coords and white square
    top_left = (int(args.top_left_x),
                int(args.top_left_y))

    bottom_right = (int(args.bottom_right_x),
                    int(args.bottom_right_y))

    target = Target(top_left, bottom_right, int(args.white_row), int(args.white_col))
    packet.target = target

    # Gather file locations
    packet.files.extend(args.images)

    """ Begin pipeline """
    processing_pipeline(packet)


if __name__ == "__main__":
    main()
