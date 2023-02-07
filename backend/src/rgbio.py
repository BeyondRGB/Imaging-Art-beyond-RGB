""" io.py
Collection of I/O functions

Functions:
    load_array(path)
    save_array(arrs, path, rm)
    load_image(path)

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import numpy as np
import rawpy as rp
from os.path import exists
from tempfile import TemporaryFile
from cv2 import cvtColor, COLOR_BayerRG2RGB


def save_image(img, path):
    """ Save image to disk
    [in] img     : image to save
    [in] path   : image save location
    """
    pass


def load_image(path):
    """ Load an image into memory
    [in] path(str) : file path to image (absolute for clarity)
    [out] numpy array containing the image
    [raise] FileNotFoundError, IOError
    """
    if not exists(path):
        raise FileNotFoundError
    # Load image
    try:
        raw = rp.imread(path)
        return raw.postprocess(use_camera_wb=False, output_bps=16).astype('f4')
    except rp._rawpy.LibRawIOError:
        raise IOError
    # Standardize based on image type
    if len(raw.shape) == 2:
        # Image is Bayer, convert to RGB, return image
        return cvtColor(raw, COLOR_BayerRG2RGB).astype('f4')
    elif len(raw.shape) == 3:
        # Image is RGB, remove any alpha channel, return image
        if raw.shape[2] == 4:
            raw[...] = raw[:, :, :3]
            return raw.astype('f4')
        elif raw.shape[2] == 3:
            return raw.astype('f4')
    # Unexpected image shape
    raise IOError


def save_array(arrs, path):
    """ Save a ndarray pair to disk
    [in]      arrs  : tuple containing ndarray pair
    [in]      path : file path to save location
    [raise] IOError
    """
    path.seek(0)
    try:
        np.savez(path, arrs[0], arrs[1])
    except rp._rawpy.LibRawIOError:
        raise IOError


def load_array(path):
    """ Load a ndarray pair into memory
    [in] path : file path of ndarray pair to be loaded
    [out] tuple containing ndarray pair
    [raise] IOError
    """
    path.seek(0)
    try:
        arrs = np.load(path, allow_pickle=True)
    except rp._rawpy.LibRawIOError:
        raise IOError
    return (arrs['arr_0'], arrs['arr_1'])


def create_temp_file():
    """ Create a temp file for array saving """
    return TemporaryFile()
