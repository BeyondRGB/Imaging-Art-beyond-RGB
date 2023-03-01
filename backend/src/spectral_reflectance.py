""" spectral_reflectance.py

Authors:
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


def spectrally_transform(packet: Packet):
    r_reference = np.genfromtxt('NGT_spectral_reflectance.csv', delimiter=',')
    r_reference[...] = np.reshape(r_reference, (36, 130))
    initial_guess = np.ndarray.flatten(INIT_M_REFL)
    opt = fmin(func=__eq, x0=initial_guess, args=(packet.camsigs, r_reference))
    print('Spectral transformation fopt: ' + str(__eq(opt, packet.camsigs, r_reference)))
    # TODO use above results
    return


def __eq(x: np.ndarray, camsigs: np.ndarray, r_reference: np.ndarray) -> float:
    """ Spectral Reflectance equation to minimize
    [in] x : The current guess array
    [in] camsigs : The camera signals extracted from the preprocessed color target
    [r_reference] : The 'actual' reference data for the given color target
    """

    x = np.reshape(x, (36, 6))
    r_camera = np.matmul(x, camsigs)

    global_maximums = []
    global_minimums = []
    global_maximums_ref = []
    global_minimums_ref = []
    for i in range(0, 36):
        row = r_camera[i]
        global_maximums.append(np.amax(row))
        global_minimums.append(np.amin(row))

        row = r_reference[i]
        global_maximums_ref.append(np.amax(row))
        global_minimums_ref.append(np.amin(row))

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
