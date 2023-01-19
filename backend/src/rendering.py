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


def render(packet):
    """ Render image pair as a single color calibrated image
    [in]     imgs   : tuple of images to render
    [in]     M      : calibration ndarray
    [out] rendered image
    [post] rendered image is loaded in memory
    """
    # Load in the image
    packet.get_subjects()

    # Render
    pass
