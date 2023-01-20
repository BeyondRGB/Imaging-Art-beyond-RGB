""" rendering.py
Collection of functions for rendering an image

Functions:
    render(timgs)

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python Imports
import gc
import numpy as np

# Local Imports
from constants import M


def render(packet):
    """ Render image pair as a single color calibrated image
    [out] rendered image
    [post] rendered image is loaded in memory
    """
    # Load in the image and get image dims
    packet.load_subject()
    subj = packet.get_subject()  # Do not use once schanells exists
    h, w, d = subj[0].shape

    # Generate schanells
    schanells = _generate_scannels(packet)

    # Render
    m = np.resize(packet.x[0:17], (3, 6))
    o = np.resize(packet.x[18:], (6, 1))
    # Subtract offsets
    schanells[...] -= o
    # Apply transformation matrix
    res = np.matmul(m, schanells)
    # Clear unneeded memory
    del schanells
    gc.collect()
    # Apply ProPhoto conversion
    res[...] = np.matmul(M, res)
    # TODO Apply Gamma
    packet.rendered_subj = np.resize(res.transpose(), (h, w, d))

    # Cleanup
    del res
    gc.collect()


def _generate_scannels(packet):
    """ Combine subject pair into a single 6 channel image
    [in] packet : pipeline packet
    [out] 6 channel subject image (R1, G1, B1, R2, G2, B2)
    [post] subject pair is removed from memory
    """
    subj = packet.get_subject()
    h, w, d = subj[0].shape

    schanells = np.ndarray((6, subj[0].size))
    schanells[0, :] = np.resize(subj[0][:, :, 0], (1, subj[0].size))
    schanells[1, :] = np.resize(subj[0][:, :, 1], (1, subj[0].size))
    schanells[2, :] = np.resize(subj[0][:, :, 2], (1, subj[0].size))
    schanells[3, :] = np.resize(subj[1][:, :, 0], (1, subj[1].size))
    schanells[4, :] = np.resize(subj[1][:, :, 1], (1, subj[1].size))
    schanells[5, :] = np.resize(subj[1][:, :, 2], (1, subj[1].size))

    packet.unload_subject()

    return schanells
