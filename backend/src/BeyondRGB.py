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


def main():
    """ App entry point """

    args = parse_args()

    if args.mode == 'gui':
        # Not Implemented
        print('This feature is not yet implemented')
        sys.exit(1)
    elif args.mode == 'tui':
        packet = tui(args)
        exit(0)
    elif args.mode == 'cli':
        # TODO packet generating bit needs to be redone
        # Gather target coords and white square
        top_left = (int(args.top_left_x), int(args.top_left_y))
        bottom_right = (int(args.bottom_right_x), int(args.bottom_right_y))

        target = gentarget((top_left, bottom_right),
                           (int(args.white_row), int(args.white_col)),
                           targ2ttype[args.target])

        # Setup packet
        packet = build_packet(args.images, target, args.outpath)

    # Begin pipeline
    processing_pipeline(packet)


def build_packet(images, target, outpath):
    """ Create packet
    [in] image  : image files
    [in] target : target grid
    [out] packet
    """
    packet = genpacket(images, target, outpath)
    return packet


if __name__ == "__main__":
    main()
