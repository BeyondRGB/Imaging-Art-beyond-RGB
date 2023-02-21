""" constants.py
Collection of global constants

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""

# File/array index constants
TARGET_A_IDX = 0
TARGET_B_IDX = 1
WHITE_A_IDX = 2
WHITE_B_IDX = 3
DARK_A_IDX = 4
DARK_B_IDX = 5
RENDERABLES_START = 6

# Swap space index constants
WHITE_SWAP_IDX = 0
DARK_SWAP_IDX = 1
TARGET_SWAP_IDX = 2
NUM_SWAP_PAIRS = 3
NUM_SWAP_FILES = NUM_SWAP_PAIRS * 2

# Dead pixel correction
BLUR_FACTOR = 3  # Must be odd and greater than 2

# Target
TARGET_RADIUS = 10

# Flat fielding
Y_VAL = 0.86122  # TODO UNHARDCODE


# Registration Constants
ALIGN = 1
REFERENCE = 0

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
        Additional Images... (A and B)
    '''
INIT_MOARR = [0.10, 0.10, 0.25, 0.50, 0.10, 0.10,
              0.10, 0.10, 0.25, 0.10, 1.00, 0.10,
              0.10, 0.10, 0.25, 0.10, 0.10, 0.50,
              0.01, 0.01, 0.01, 0.01, 0.01, 0.01]

# Rendering
PROPHOTO_TRANS = [[1.34594330, -0.2556075, -0.0511118],
                  [-0.5445989, 1.50816730, 0.02053510],
                  [0.00000000, 0.00000000, 1.21181280]]
SRGB_TRANS = [[3.13385610, -1.6168667, -0.4906146],
              [-0.9787684, 1.91614150, 0.03345400],
              [0.07194530, -0.2289914, 1.40524270]]
COLORSPACE = 'sRGB'  # 'sRGB' or ProPhoto
