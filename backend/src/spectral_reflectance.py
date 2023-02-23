""" spectral_reflectance.py

Authors:
    Keenan Miller <https://github.com/keenanm500>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python Imports
from scipy.optimize import fmin
from scipy.linalg import pinv
import numpy as np

# Local Imports
from constants import INIT_M_REFL


def spectrally_transform(packet):

    initial_guess = np.ndarray.flatten(INIT_M_REFL)
    res = fmin(func=eq, x0=initial_guess, args=(packet.camsigs, None))


def eq(x, camsigs, nothing):
    """ Equation to minimize """

    x = np.reshape(x, (36, 6))
    r_camera = np.matmul(x, camsigs)

    global_maximums = []
    global_minimums = []
    for i in range(0, 36):
        row = r_camera[i]
        global_maximums.append(np.amax(row))
        global_minimums.append(np.amin(row))
    print(str(global_maximums))
    print(str(global_minimums))

    return 0
