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

import cv2
import numpy as np
import rawpy as rp
from os.path import exists
from tempfile import TemporaryFile


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
        img[...] = cv2.cvtColor(img,  cv2.COLOR_RGB2BGR)
        cv2.imwrite(out, img)
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
        raw = rp.imread(path)
        rgb = cv2.cvtColor(raw.raw_image, cv2.COLOR_BayerRGGB2RGB).astype('f4')
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
