""" calibration.py
Collection of functions for color calibration

Functions:
    color_calibrate(timgs)

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
from scipy.optimize import minimize


def color_calibrate(packet):
    """ Process target images to get the calibrated M matrix
    Procedure:
        Generate camsigs array with all channels
        Generate initial minimization variables
        Minimize xyz equation
    [in] packet : pipeline packet
    [out] calibrated M ndarray
    """
    _extract_camsigs(packet)

    # Save memory (we'll reload later)
    packet.unload_targets()

    res = minimize(xyzequation, moarr, (camsigs, labref), method='Powell')
    pass


def _extract_camsigs(packet):
    """ Generate camsigs array
    [in] packet : pipeline packet
    [post] packet camsigs array is populated
    """
