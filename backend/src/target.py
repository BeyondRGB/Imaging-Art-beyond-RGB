""" target.py
Target class definition

Classes:
    Target

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""


class Target:
    """ Target
    Class representing the calibration target

    Methods:
        get_center_coord : Get the center of a target square
    """
    _tleft = ()  # top leftx,y
    _bright = ()  # bottom right x,y
    _wrow = 0  # white row (0 indexed)
    _wcol = 0  # white col (0 indexed)
    _num_rows = 10
    _num_cols = 13

    def __init__(self, top_left, bottom_right, white_row, white_col):
        """ Class init function
        [in] top_left : the top left coordinate of the target (x, y)
        [in] bottom_right : the bottoml right coordinate of the target (x, y)
        [in] white_row    : white row (0 indexed)
        [in] white_col    : white col (0 indexed)
        """
        self._tleft = top_left
        self._bright = bottom_right
        self._wrow = white_row
        self._wcol = white_col

    def get_center_coord(self, row, col):
        """ Get the center of a target square
        [in] row : target square row
        [in] col : target square column
        [out] center coordinate of target square
        """
        # Calculate offset
        x = (col*2+1)*((self._bright[0]-self._tleft[0])/(self._num_cols*2))
        y = (row*2+1)*((self._bright[1]-self._tleft[1])/(self._num_rows*2))

        # Calculate true center
        x += self._tleft[0]
        y += self._tleft[1]

        return int(x), int(y)

    def get_white(self):
        """ Get the white space row and col
        [out] row, col
        """
        return self._wrow, self._wcol

    def get_dims(self):
        """ Get dimentions of target
        [out] target dimentions
        """
        return self._num_cols, self._num_rows
