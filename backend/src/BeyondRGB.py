""" preprocessing.py
App init functions

Functions:
    main

Authors:
    Brendan Grau <https://github.com/Victoriam7>
    Keenan Miller <https://github.com/keenanm500>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import sys

# Local imports
from tui.tui import tui
from utils.parser import parse_args
from calibration.packet import genpacket, gentarget
from calibration.pipeline import processing_pipeline
from calibration.constants import TARGETTYPE_NGT, TARGETTYPE_APT,\
                                  TARGETTYPE_CCSG, TARGETTYPE_CC

# Target arg to TARGETTYPE translator
targ2ttype = {'NGT': TARGETTYPE_NGT,
              'APT': TARGETTYPE_APT,
              'CCSG': TARGETTYPE_CCSG,
              'CC': TARGETTYPE_CC}


# Usage help messages
TARGET_TYPE_HELP = 'The color target reference data; defaults to NGT'
TOP_LEFT_X_HELP = 'Pixel value of the top-left of the upright color target from the left end of the color-target image'
BOTTOM_RIGHT_X_HELP = 'Pixel value of the bottom-right of the upright color target from the left end of the color-target image'
TOP_LEFT_Y_HELP = 'Pixel value of the top-left of the upright color target from the top end of the color-target image'
BOTTOM_RIGHT_Y_HELP = 'Pixel value of the bottom-right of the upright color target from the top end of the color-target image'
WHITE_COL_HELP = 'Column of selected white patch from left of target (0 indexed)'
WHITE_ROW_HELP = 'Row of selected white patch from top of target (0 indexed)'
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
OUTPATH_HELP = 'Output directory'
COLORSPACE_HELP = 'Color space for output images. (ProPhoto or sRGB)'


def main():
    """ App entry point """

    args = parse_args()

    if args.mode == 'gui':
        # Not Implemented
        print('This feature is not yet implemented')
        sys.exit(1)
    elif args.mode == 'tui':
        packet, outpath = tui(args)
    elif args.mode == 'cli':
        # TODO packet generating bit needs to be redone
        # Gather target coords and white square
        top_left = (int(args.top_left_x), int(args.top_left_y))
        bottom_right = (int(args.bottom_right_x), int(args.bottom_right_y))
        target = gentarget((top_left, bottom_right),
                           (int(args.white_row), int(args.white_col)),
                           targ2ttype[args.target])
        packet = genpacket(args.images, target)
        outpath = args.outpath

    # Begin pipeline
    processing_pipeline(packet, outpath, "sRGB")


if __name__ == "__main__":
    main()
