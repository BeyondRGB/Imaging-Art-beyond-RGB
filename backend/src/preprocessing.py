""" preprocessing.py
Collection of functions used for preprocessing

Functions:
    preprocess(imgs)
    bit_scale(imgs)
    dark_current_correction(imgs, dimgs)
    flat_field_w_gen(timgs, wimgs)
    flat_fielding(imgs, wimgs, ws)
    registration(imgs)

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import numpy as np


def preprocess(imgs):
    """ Run preprocessing on pair of images in place
    [in] imgs : tuple containing image pair
    [post] images preprocessed in place
    """
    pass


def bit_scale(imgs):
    """ Bit scale pair of images in place
    [in] imgs : tuple containing image pair
    [post] images bit scaled in place
    """
    pass


def dark_current_correction(imgs, dimgs):
    """ Dark current correct pair of images in place
    [in] imgs  : tuple containing image pair
    [in] dimgs : tuple containing dark image pair
    [post] images dark current corrected in place
    """
    pass


def flat_field_w_gen(timgs, wimgs):
    """ Generate flat fielding w value
    [in] timgs : tuple containing target image pair
    [in] wimgs : tuple containing white image pair
    [out] tuple containing w value pair (w1, w2)
    """
    pass


def flat_fielding(imgs, wimgs, ws):
    """ Flat field image pair in place
    [in] imgs  : tuple containing image pair
    [in] wimgs : tuple containing white image pair
    [in] ws    : tuple containing w value pair
    """
    pass


def registration(imgs):
    """ Register image pair in place
    [in] imgs  : tuple containing image pair
    [post] images registered in place
    """
    pass
