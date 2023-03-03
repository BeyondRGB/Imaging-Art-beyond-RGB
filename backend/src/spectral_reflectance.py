""" spectral_reflectance.py
Funtions for getting the spectral transformation array

Authors:
    Brendan Grau <https://github.com/Victoriam7>
    Keenan Miller <https://github.com/keenanm500>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python Imports
from scipy.optimize import fmin
import numpy as np

# Local Imports
from packet import Packet

# Initial guess array
INIT_M_REFL = np.full((36, 6), .2)


def spectrally_transform(packet: Packet, camsigs: np.ndarray):
    """ Spectral Reflectance minimization runner
    [in] packet  : The processing packet
    [in] camsigs : The camera signals extracted from the color target
    """
    r_reference = packet.target.r_ref
    initial_guess = np.ndarray.flatten(INIT_M_REFL)

    opt = fmin(func=__eq, x0=initial_guess, args=(camsigs, r_reference))
    packet.m_refl_matrix = opt
    print('Spectral transformation fopt: ' + str(__eq(opt, camsigs, r_reference)))

    # TODO use above results
    return


def __eq(x: np.ndarray, camsigs: np.ndarray, r_reference: np.ndarray) -> float:
    """ Spectral Reflectance equation to minimize
    [in] x            : The current guess array
    [in] camsigs      : The camera signals extracted from the color target
    [in] r_reference  : The 'actual' reference data for the given color target
    [out]             : The resulting Z value, lower is better
    """

    x = np.reshape(x, (36, 6))
    r_camera = np.matmul(x, camsigs)

    global_maximums = np.amax(r_camera, axis=1)
    global_minimums = np.amin(r_camera, axis=1)
    global_maximums_ref = np.amax(r_reference, axis=1)
    global_minimums_ref = np.amin(r_reference, axis=1)

    r_diff = np.subtract(r_reference, r_camera)
    r_diff_square = np.square(r_diff)
    e1 = np.sum(r_diff_square)**.5

    r_max_diff = np.subtract(global_maximums, global_maximums_ref)
    r_max_diff_square = np.square(r_max_diff)
    e2 = np.sum(r_max_diff_square)

    r_min_diff = np.subtract(global_minimums, global_minimums_ref)
    r_min_diff_square = np.square(r_min_diff)
    e3 = np.sum(r_min_diff_square)

    Z = e1 + 10*e2 + 50*e3

    return Z
