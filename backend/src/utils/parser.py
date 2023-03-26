""" parser.py

Classes:
    Parser : This parser overrides argparse to allow for automatic
                printing of the help message when a user inputs invalid
                arguments.

Authors:
    Keenan Miller <https://github.com/keenanm500>
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import sys
import argparse

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
TUI_HELP = 'Enable TUI mode'
SUB_PARSER_HELP = 'Select visual mode'


# Arg parsing functions
def parse_args() -> list:
    """ Gather arguments from the command line
    Subparsers are used to allow both gui and tui modes to not have any
    additional arguments. This also allows seperate arguments for each mode in
    the future.
    [out] the list of parsed arguments
    """
    # Define parsers
    parser = Parser(formatter_class=argparse.RawTextHelpFormatter)
    subparsers = parser.add_subparsers(help=SUB_PARSER_HELP, dest='mode',
                                       required=True)
    gui = subparsers.add_parser('gui')
    tui = subparsers.add_parser('tui')
    cli = subparsers.add_parser('cli')

    # cli arguments
    ttypes = ['NGT', 'APT', 'CCSG', 'CC']  # Target types
    cli.add_argument('-t', '--target', choices=ttypes, default='NGT', help=TARGET_TYPE_HELP)
    cli.add_argument('top_left_x', help=TOP_LEFT_X_HELP)
    cli.add_argument('bottom_right_x', help=BOTTOM_RIGHT_X_HELP)
    cli.add_argument('top_left_y', help=TOP_LEFT_Y_HELP)
    cli.add_argument('bottom_right_y', help=BOTTOM_RIGHT_Y_HELP)
    cli.add_argument('white_col', help=WHITE_COL_HELP)
    cli.add_argument('white_row', help=WHITE_ROW_HELP)
    cli.add_argument('images', nargs='+', help=IMAGES_HELP)
    cli.add_argument('--outpath', help=OUTPATH_HELP)

    args = parser.parse_args()
    return args


def __validate(parser: argparse.ArgumentParser, args: dict):
    if len(args.images) < 6:
        parser.print_help()
        sys.exit(1)


# Class to override argparse
class Parser(argparse.ArgumentParser):
    """ Custom parser to allow for a custom error message """

    def error(self, message):
        self.print_help()
        sys.exit(2)
