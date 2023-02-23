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

# Constants
PROPHOTO_Y_LINE = 0.001953125
SRGB_Y_LINE = 0.0031308



def render(packet):
    """ Render image pair as a single color calibrated image
    [in] packet : pipeline packet
    [out] rendered image
    [post] rendered image is loaded in memory
    [post] camsigs deleted from packet
    """
    camsigs = __genimgsigs(packet)

    # Compute color calibrated image
    m = np.resize(packet.x[0:18], (3, 6))
    o = np.resize(packet.x[18:], (6, 1))
    xyz = np.matmul(m, np.subtract(camsigs, o))
    del camsigs
    gc.collect()

    # Convert to ProPhoto color space
    if COLORSPACE == 'ProPhoto':
        rgb = np.matmul(PROPHOTO_TRANS, xyz)
        del xyz
        gc.collect()
        np.clip(rgb, 0, 1, out=rgb)
        # Apply Gamma
        np.piecewise(rgb, [rgb > PROPHOTO_Y_LINE, rgb <= PROPHOTO_Y_LINE],
                     [lambda rgb: rgb ** (1/1.8), lambda rgb: rgb * 16])

    # Convert to sRGB color space
    if COLORSPACE == 'sRGB':
        rgb = np.matmul(PROPHOTO_TRANS, xyz)
        del xyz
        gc.collect()
        np.clip(rgb/100, 0, 1, out=rgb)  # TODO remove divide by 100
        # Apply Gamma
        np.piecewise(rgb, [rgb > SRGB_Y_LINE, rgb <= SRGB_Y_LINE],
                     [lambda rgb: (1.055 * (rgb ** (1/2.4))) - 0.055,
                      lambda rgb: 12.92 * rgb])

    # Reshape into image
    render = np.dstack((rgb[0], rgb[1], rgb[2])).reshape(packet.dims)
    print(render)
    del rgb
    gc.collect()

    packet.render = np.float32(render)


def __genimgsigs(packet):
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
