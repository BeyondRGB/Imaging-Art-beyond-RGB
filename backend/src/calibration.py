""" calibration.py
Collection of functions for color calibration

Functions:
    color_calibrate(packet)

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import gc
import numpy as np
from scipy.optimize import fmin

from lab_refs import LAB_REF
from spectral_equation import xyztolab, ciede2000

# CONSTANTS
__INIT_MOARR = [0.10, 0.10, 0.25, 0.50, 0.10, 0.10,
                0.10, 0.10, 0.25, 0.10, 1.00, 0.10,
                0.10, 0.10, 0.25, 0.10, 0.10, 0.50,
                0.01, 0.01, 0.01, 0.01, 0.01, 0.01]


def color_calibrate(packet):
    """ Process target images to get the calibrated M matrix
    Procedure:
        Generate camsigs array with all channels
        Generate initial minimization variables
        Minimize xyz equation
    [in] packet : pipeline packet
    [out] calibrated M ndarray
    [post] target image is unloaded
    [post] packet x variable is populated
    """
    res = fmin(__de_equ, __INIT_MOARR, (packet.camsigs, LAB_REF))
    print(__de_equ(res, packet.camsigs, LAB_REF))
    packet.x = res


def __de_equ(x, camsigs, labref):
    """ ∆E equation for color tranformation matrix optimization
    [in] x : current input parameters guess
    [in] camsigs : target camera signals we are modifying
    [in] labref  : target reference data
    [out] average error
    """
    m = np.resize(x[0:18], (3, 6))
    o = np.resize(x[18:], (6, 1))
    xyz = np.matmul(m, np.subtract(camsigs, o))
    lab = xyztolab(xyz)
    xyzshape = xyz.shape[1]
    del xyz
    gc.collect()
    ciede = np.zeros(xyzshape)
    for i in range(0, lab.shape[1]):
        ciede[i] = ciede2000(lab[:, i], labref[:, i])
    err = np.mean(ciede)
    del ciede
    gc.collect()
    return err
