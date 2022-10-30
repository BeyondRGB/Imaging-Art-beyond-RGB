""" io.py
Collection of I/O functions

Functions:
    load_array(path)
    save_array(arrs, path, rm)
    load_image(path)

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import numpy as np
import rawpy as rp
from os.path import exists
from tempfile import TemporaryFile


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
    try:
        raw = rp.imread(path)
    except rp._rawpy.LibRawIOError:
        raise IOError
    # TODO investigate possible memmory usage issues
    return np.array(raw.postprocess(no_auto_bright=True, output_bps=16), dtype='f4')


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
