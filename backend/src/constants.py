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
Y_VAL = 0.86122  # TODO CHECK IF CORRECT VALUE

# Calibration
INIT_M = [0.10, 0.10, 0.25, 0.50, 0.10, 0.10,
          0.10, 0.10, 0.25, 0.10, 1.00, 0.10,
          0.10, 0.10, 0.25, 0.10, 0.10, 0.50]
INIT_O = [0.01, 0.01, 0.01, 0.01, 0.01, 0.01]
INIT_MOARR = INIT_M + INIT_O
