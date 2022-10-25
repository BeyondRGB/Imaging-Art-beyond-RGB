""" io.py
Collection of I/O functions

Functions:
    load_array(fpath)
    save_array(arrs, fpath, rm)
    load_image(fpath)

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import numpy as np
import rawpy as rp
import gc


def save_image(img, fpath):
    """ Save image to disk
    [in] img   : image to save
    [in] fpath : image save location
    """

def load_image(fpath):
    """ Load an image into memory
    [in] fpath(str) : file path to image (absolute for clarity)
    [out] numpy array containing the image
    """
    pass


def save_array(arrs, fpath, rm=False):
    """ Save a ndarray pair to disk
    [in]      arrs  : tuple containing ndarray pair
    [in]      fpath : file path to save location
    [in, opt] rm    : whether to delete the arrays from memory
    [post] Arrays deleted from memory if rm is True
    """
    pass


def load_array(fpath):
    """ Load a ndarray pair into memory
    [in] fpath : file path of ndarray pair to be loaded
    [out] tuple containing ndarray pair
    """
    pass


