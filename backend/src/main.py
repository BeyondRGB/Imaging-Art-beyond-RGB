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
# Python imports
import sys
import argparse

# Local imports
from processing_pipeline import processing_pipeline
from packet import Packet
from target import Target
from parser import Parser
import constants


def main():
    # App entry point
    parser = Parser(formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('-t', '--target', choices=['NGT', 'APT', 'CCSG', 'CC'], default='NGT', help=constants.TARGET_TYPE_HELP)

    # Top left & bottom right of (NGT at the moment) target (pixels)
    parser.add_argument('top_left_x', help=constants.TOP_LEFT_X_HELP)
    parser.add_argument('bottom_right_x', help=constants.BOTTOM_RIGHT_X_HELP)
    parser.add_argument('top_left_y', help=constants.TOP_LEFT_Y_HELP)
    parser.add_argument('bottom_right_y', help=constants.BOTTOM_RIGHT_Y_HELP)

    # Location of white square on target image
    parser.add_argument('white_col', help=constants.WHITE_COL_HELP)
    parser.add_argument('white_row', help=constants.WHITE_ROW_HELP)

    parser.add_argument('images', nargs='+', help=constants.IMAGES_HELP)
    args = parser.parse_args()

    if len(args.images) < 6:
        parser.print_help()
        sys.exit(1)

    # Gather target coords and white square
    top_left = (int(args.top_left_x), int(args.top_left_y))
    bottom_right = (int(args.bottom_right_x), int(args.bottom_right_y))

    target = Target(top_left, bottom_right, int(args.white_row), int(args.white_col))

    # Setup packet
    packet = build_packet(args.images, target)

    # Begin pipeline
    processing_pipeline(packet)


def build_packet(images, target):
    packet = Packet()
    packet.files.extend(images)
    packet.target = target
    return packet


if __name__ == "__main__":
    main()
