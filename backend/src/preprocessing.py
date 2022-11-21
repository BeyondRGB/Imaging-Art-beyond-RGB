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
import cv2


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
