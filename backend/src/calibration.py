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
# Python Imports
import gc
import time
import numpy as np
from scipy.optimize import fmin, minimize

# Local Imports
from lab_refs import LAB_REF
from constants import INIT_MOARR, TARGET_RADIUS
from spectral_equation import xyztolab, ciede2000


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
    #res = minimize(_xyz, INIT_MOARR, (packet.camsigs, LAB_REF), method='Powell')
    res = fmin(_xyz, INIT_MOARR, (packet.camsigs, LAB_REF))
    print(res)

    # TODO ERROR CHECKING

    packet.x = res.x


def _xyz(x, camsigs, labref):
    """ Equation for color calibrator to minimize """
    m = np.resize(x[0:17], (3, 6))
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
