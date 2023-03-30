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
import sys

import numpy as np
import rawpy as rp
from os.path import exists
from tempfile import TemporaryFile
from cv2 import imwrite, cvtColor, COLOR_RGB2BGR, COLOR_BayerRGGB2RGB


def save_image(img: np.ndarray, path: str, filename: str):
    """ Save image to disk
    [in] img :      image data
    [in] path :     path to save file to
    [in] filename : file name to save to
    """
    if path:
        out = path + '/' + filename + ".tiff"
    else:
        out = './' + filename + ".tiff"
    try:
        img[...] = cvtColor(img,  COLOR_RGB2BGR)
        imwrite(out, img)
    except PermissionError:
        print("Failed to create file at: " + out + " (insufficient permissions)")
        sys.exit(1)


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
        raw = rp.imread(path).raw_image
        rgb = cvtColor(raw, COLOR_BayerRGGB2RGB).astype('f4')
        return rgb
    except rp._rawpy.LibRawIOError:
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
    return (arrs['arr_0'].astype('f4'), arrs['arr_1'].astype('f4'))


def create_temp_file():
    """ Create a temp file for array saving """
    return TemporaryFile()
