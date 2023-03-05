""" verification.py

Authors:
    Keenan Miller <https://github.com/keenanm500> 🙃

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python Imports
import numpy as np

# Local Imports
from spectral_equation import ciede2000, xyztolab


def verify_color_transformation(packet, camsigs):
    """ Verify color calibration matrix
    [in] packet     : pipeline packet
    [in] camsigs    : The camera signals extracted from the color target
    [out] xyz_camera
    [out] lab_camera
    [out] delta E array
    """
    m = np.resize(packet.mo_matrix[0:18], (3, 6))
    o = np.resize(packet.mo_matrix[18:], (6, 1))
    xyz = np.matmul(m, np.subtract(camsigs, o))
    lab = xyztolab(xyz)
    xyzshape = xyz.shape[1]
    ciede = np.zeros(xyzshape)
    for i in range(0, lab.shape[1]):
        ciede[i] = ciede2000(lab[:, i], packet.target.lab_ref[:, i])

    return xyz, lab, ciede


def verify_spectral_transformation(packet, camsigs):
    """ Verify spectral transformation matrix
    [in] packet     : pipeline packet
    [in] camsigs    : The camera signals extracted from the color target
    [out] r_camera
    [out] RMSE
    """
    m_refl_matrix = np.reshape(packet.m_refl_matrix, (36, 6))
    r_camera = np.matmul(m_refl_matrix, camsigs)
    r_diff = np.subtract(r_camera, packet.target.r_ref)
    r_diff_sq = np.square(r_diff)
    avg = np.average(r_diff_sq)
    RMSE = avg**.5

    return r_camera, RMSE
