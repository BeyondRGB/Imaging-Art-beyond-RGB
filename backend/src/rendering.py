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
# Python imports
import numpy as np
import gc

# Local imports
from constants import PROPHOTO_TRANS, COLORSPACE


def render(packet):
    """ Render image pair as a single color calibrated image
    [in] packet : pipeline packet
    [out] rendered image
    [post] rendered image is loaded in memory
    [post] camsigs deleted from packet
    """
    camsigs = __gensubjcamsigs(packet)

    # Compute color calibrated image
    m = np.resize(packet.x[0:18], (3, 6))
    o = np.resize(packet.x[18:], (6, 1))
    xyz = np.matmul(m, np.subtract(camsigs, o))
    del camsigs
    gc.collect()

    # Convert to ProPhoto color space
    if COLORSPACE ==:
        rgb_pp = np.matmul(PROPHOTO_TRANS, xyz)
        del xyz
        gc.collect()

    # Convert to sRGB color space
    if True:

    # Clip Values
    np.clip(rgb_pp/100, 0, 1, out=rgb_pp)

    # Reshape into image
    render = np.dstack((rgb_pp[0], rgb_pp[1], rgb_pp[2])).reshape(packet.dims)
    del rgb_pp
    gc.collect()

    packet.render = np.float32(render)


def __gensubjcamsigs(packet):
    """ Generate camsigs array for whole image
    [in] packet : pipeline packet
    [out] camsigs array
    """
    subj = packet.get_subject()

    # We'll need this later
    packet.dims = subj[0].shape

    # Python is like candy; it tastes good but I hate that I like it
    camsigs = [subj[0][:, :, 0].flatten(),
               subj[0][:, :, 1].flatten(),
               subj[0][:, :, 2].flatten(),
               subj[1][:, :, 0].flatten(),
               subj[1][:, :, 1].flatten(),
               subj[1][:, :, 2].flatten()]

    packet.unload_subject()  # Cleanup

    return camsigs
