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
