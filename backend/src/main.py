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

    """
    Whether the target is combined with the image to be processed
    """
    parser.add_argument('--combined-target-subject', action='store_true', help='Include this flag if the first image contains the color target')

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
        Target A (Required unless --combined-target-subject is True)
        Target B (Required unless --combined-target-subject is True)
        Flat Field A
        Flat Field B
        Dark Field A
        Dark Field B
        Subject A
        Subject B
        Additional Images... (A and B)
    '''
    parser.add_argument('images', nargs='+', help=images_help)
    args = parser.parse_args()

    if len(args.images) < 6 or (len(args.images) < 8 and not args.combined_target_subject):
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
    # TODO: Don't duplicate subject images
    if args.combined_target_subject:
        packet.files.extend(args.images[4:6])
    packet.files.extend(args.images)

    """ Begin pipeline """
    processing_pipeline(packet)


if __name__ == "__main__":
    main()
