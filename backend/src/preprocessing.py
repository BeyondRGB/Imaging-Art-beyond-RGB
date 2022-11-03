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
# Python imports
import numpy as np
from constants import IMAGE_A_IDX, IMAGE_B_IDX, WHITE_A_IDX, WHITE_B_IDX, \
    DARK_A_IDX, DARK_B_IDX


def preprocess(imgs, ws=(-1, -1), target=None):
    """ Run preprocessing on pair of images in place
    [in] imgs       : tuple containing image pair
    [in,opt] ws     : tuple containing w values pair
    [in,opt] target : target information, needed to generate w values
    [out] tuple containing w value pair (w1, w2)
    [post] images preprocessed in place
    """
    img_pair = imgs[IMAGE_A_IDX:IMAGE_B_IDX+1]
    dark_pair = imgs[DARK_A_IDX:DARK_B_IDX+1]
    white_pair = imgs[WHITE_A_IDX:WHITE_B_IDX+1]

    bit_scale(img_pair)
    dark_current_correction(img_pair, dark_pair)
    if ws[0] == -1 or ws[1] == -1:
        # We need to generate w values
        ws = flat_field_w_gen(img_pair, white_pair, target)
    flat_fielding(img_pair, white_pair, ws)
    return ws


def bit_scale(imgs):
    """ Bit scale pair of images in place
    [in] imgs : tuple containing image pair
    [post] images bit scaled in place
    """
    s = ((2**16 - 1)/(2**14 - 1))  # TODO determine actual scale
    imgs[0][:, :, :] = imgs[0] * s
    imgs[1][:, :, :] = imgs[1] * s


def dark_current_correction(imgs, dimgs):
    """ Dark current correct pair of images in place
    [in] imgs  : tuple containing image pair
    [in] dimgs : tuple containing dark image pair
    [post] images dark current corrected in place
    """
    imgs[0][:, :, :] = imgs[0] - dimgs[0]
    imgs[1][:, :, :] = imgs[1] - dimgs[1]


def flat_field_w_gen(timgs, wimgs, target):
    """ Generate flat fielding w value
    [in] timgs  : tuple containing target image pair
    [in] wimgs  : tuple containing white image pair
    [in] target : target information
    [out] tuple containing w value pair (w1, w2)
    """
    y = 0.86122  # TODO remove hardcoding
    t1mean = np.mean(timgs[0][1960:2040, 3040:3100], axis=(0, 1))
    t2mean = np.mean(timgs[1][1960:2040, 3040:3100], axis=(0, 1))
    w1mean = np.mean(wimgs[0][1960:2040, 3040:3100], axis=(0, 1))
    w2mean = np.mean(wimgs[1][1960:2040, 3040:3100], axis=(0, 1))
    w1 = y * (t1mean / w1mean)
    w2 = y * (t2mean / w2mean)
    return w1, w2


def flat_fielding(imgs, wimgs, ws):
    """ Flat field image pair in place
    [in] imgs  : tuple containing image pair
    [in] wimgs : tuple containing white image pair
    [in] ws    : tuple containing w value pair
    """
    imgs[0][:, :, :] = ws[0] * (imgs[0] / wimgs[0])
    imgs[1][:, :, :] = ws[1] * (imgs[1] / wimgs[1])


def registration(imgs):
    """ Register image pair in place
    [in] imgs  : tuple containing image pair
    [post] images registered in place
    """
    pass
