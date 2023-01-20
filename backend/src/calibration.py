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
from scipy.optimize import minimize

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
    camsigs = _extract_camsigs(packet)

    # Save memory (we'll reload later)
    packet.unload_target()

    t = time.perf_counter()
    res = minimize(_xyz, INIT_MOARR, (camsigs, LAB_REF), method='Powell')
    print("Minimize: " + str(time.perf_counter() - t))

    # TODO ERROR CHECKING

    packet.x = res.x


def _extract_camsigs(packet):
    """ Generate camsigs array
    [in] packet : pipeline packet
    [out] camsigs array
    """
    t_img = packet.get_target_img()
    tr = TARGET_RADIUS

    camsigs = np.ndarray((6, 130))
    siglist = _gen_siglist(packet)
    for i, sig in enumerate(siglist):
        cell = t_img[0][sig[1]-tr:sig[1]+tr, sig[0]-tr:sig[0]+tr]
        avg = np.average(cell, axis=(0, 1))
        camsigs[0, i] = avg[0]
        camsigs[1, i] = avg[1]
        camsigs[2, i] = avg[2]
        cell = t_img[1][sig[1]-tr:sig[1]+tr, sig[0]-tr:sig[0]+tr]
        avg = np.average(cell, axis=(0, 1))
        camsigs[3, i] = avg[0]
        camsigs[4, i] = avg[1]
        camsigs[5, i] = avg[2]

    return camsigs


def _gen_siglist(packet):
    """ Generate the list of target points
    [in] packet : pipeline packet
    [out] list of target points
    """
    target = packet.target
    cols, rows = target.get_dims()

    siglist = []
    for r in range(0, rows):
        for c in range(0, cols):
            siglist.append(target.get_center_coord(r, c))
    return siglist


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
