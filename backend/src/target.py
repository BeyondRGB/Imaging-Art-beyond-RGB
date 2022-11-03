""" target.py
Target class definition

Classes:
    Target

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LINSE.txt for details)
"""


class Target:
    """ Target
    Structure for storing details of the user selected target

    Members:
        target_type
        cells
        white_idx
    """
    targe_type = -1     # Type of target
    cells = []          # list of (x1, x2, y1, y2)'s
    white_idx = -1      # white cell index
