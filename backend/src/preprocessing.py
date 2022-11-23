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
from cv2 import medianBlur

# Local imports
from constants import IMAGE_A_IDX, IMAGE_B_IDX, WHITE_A_IDX, WHITE_B_IDX, \
    DARK_A_IDX, DARK_B_IDX


def preprocess(imgs, ws=(-1, -1), target=None):
    """ Run preprocessing on pair of images in place
    [in] imgs       : tuple containing all image pairs
    [in,opt] ws     : tuple containing w values pair
    [in,opt] target : target information, needed to generate w values
    [out] tuple containing w value pair (w1, w2) for use later
    [out] touple containing location of dead pixels
    [post] images preprocessed in place
    [raise] ZeroDivisionError
    """
    img_pair = imgs[IMAGE_A_IDX:IMAGE_B_IDX+1]
    dark_pair = imgs[DARK_A_IDX:DARK_B_IDX+1]
    white_pair = imgs[WHITE_A_IDX:WHITE_B_IDX+1]

    bit_scale(white_pair)
    bit_scale(dark_pair)

    dead_pixel_correction(imgs)

    dark_current_correction(white_pair, dark_pair)

    bit_scale(img_pair)
    dark_current_correction(img_pair, dark_pair)
    if ws[0] == -1 or ws[1] == -1:
        # We need to generate w values
        ws = flat_field_w_gen(img_pair, white_pair, target)
    # flat_fielding(img_pair, white_pair, ws)
    return ws


def dead_pixel_correction(imgs, dead_set=()):
    """ Correct for dark pixels
    [in] imgs         : tuple containing all image pairs
    [in,opt] dead_set : tuple of arrays of the location of dead pixels
    [out] tuple of arrays of the location of dead pixels
    [post] images corrected for dead pixels
    """
    img_pair = imgs[IMAGE_A_IDX:IMAGE_B_IDX+1]
    dark_pair = imgs[DARK_A_IDX:DARK_B_IDX+1]
    white_pair = imgs[WHITE_A_IDX:WHITE_B_IDX+1]
    img_pair[0][...] = medianBlur(img_pair[0], 3)
    img_pair[1][...] = medianBlur(img_pair[1], 3)
    dark_pair[0][...] = medianBlur(dark_pair[0], 3)
    dark_pair[1][...] = medianBlur(dark_pair[1], 3)
    white_pair[0][...] = medianBlur(white_pair[0], 3)
    white_pair[1][...] = medianBlur(white_pair[1], 3)


def bit_scale(imgs):
    """ Bit scale pair of images in place
    [in] imgs : tuple containing image pair
    [post] images bit scaled in place
    """
    s = ((2**16 - 1)/(2**14 - 1))  # TODO determine actual scale
    imgs[0] *= s
    imgs[1] *= s


def dark_current_correction(imgs, dimgs):
    """ Dark current correct pair of images in place
    [in] imgs  : tuple containing image pair
    [in] dimgs : tuple containing dark image pair
    [post] images dark current corrected in place
    """
    imgs[0] -= dimgs[0]
    imgs[1] -= dimgs[1]


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
    [raise] ZeroDivisionError
    """
    imgs[0] /= wimgs[0]
    imgs[0] *= ws[0]
    imgs[1] /= wimgs[1]
    imgs[1] *= ws[1]


def registration(imgs):
    """ Register image pair in place
    [in] imgs  : tuple containing image pair
    [post] images registered in place
    """
    # TODO how are these gonna be and what to return
    reference_color, align_color = imgs

    # grayscale
    img1 = cv2.cvtColor(reference_color, cv2.COLOR_BGR2GRAY)
    img2 = cv2.cvtColor(align_color, cv2.COLOR_BGR2GRAY)
    height, width = img2.shape

    # create ORB detector
    orb_detector = cv2.ORB_create(5000)
    key_points1, descriptors1 = orb_detector.detectAndCompute(img1, None)
    key_points2, descriptors2 = orb_detector.detectAndCompute(img2, None)

    # match images
    matches = cv2.BFMatcher.match(descriptors1, descriptors2)
    matches = tuple(sorted(matches, key=lambda x: x.distance))
    matches = matches[:int(len(matches)) * 0.9]
    num_matches = matches

    p1 = np.zeros((num_matches, 2))
    p2 = np.zeros((num_matches, 2))

    for i in range(len(matches)):
        p1[i, :] = key_points1[matches[i].queryIdx].pt
        p2[i, :] = key_points2[matches[i].trainIdx].pt

    homography, mask = cv2.findHomography(p1, p2, cv2.RANSAC)

    transformed_image = cv2.warpPerspective(reference_color, homography, (width, height))

    return transformed_image
