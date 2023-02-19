""" util.py
Utility functions required by multiple subsystems that do not fit cleanly into
the fine naming system

Functions:
    extract_camsigs()
    gen_siglist()

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import numpy as np

# Local imports
from constants import TARGET_RADIUS


def extract_camsigs(packet):
    """ Generate camsigs array
    [in] packet : pipeline packet
    [out] camsigs array
    """
    t_img = packet.get_target_img()
    tr = TARGET_RADIUS

    # TODO fix orientation
    camsigs = np.ndarray((6, 130))
    siglist = packet.target.gen_siglist()
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


