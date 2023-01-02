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

    Members:
        coords       : Corners of the target [(top left), (bottom right)]
        white_square : White square location (row, col)
        num_rows     : number of rows in target
        num_rows     : number of cols in target

    Methods:
        get_center_coord : Get the center of a target square
    """
    coords = [(), ()]
    white_square = ()
    num_rows = 13
    num_cols = 10

    def get_center_coord(self, row, col):
        """ Get the center of a target square
        [in] row : target square row
        [in] col : target square column
        [out] center coordinate of target square
        """
        # Calculate offset
        x = (col+1)*(((self.coords[1][0]-self.coords[0][0])/self.num_cols)/2)
        y = (row+1)*(((self.coords[1][1]-self.coords[0][1])/self.num_rows)/2)

        # Calculate true center
        x += self.coords[0][0]
        y += self.coords[0][1]

        return int(x), int(y)
