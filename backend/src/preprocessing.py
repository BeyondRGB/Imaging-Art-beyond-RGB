""" preprocessing.py
Collection of functions used for preprocessing

Functions:
    preprocess(packet)
    bit_scale(packet)
    dark_current_correction(packet, dimgs)
    flat_field_w_gen(packet)
    flat_fielding(packet)
    registration(packet)

Authors:
    Brendan Grau <https://github.com/Victoriam7>
    Elijah Cooper

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
from time import sleep

# Python imports
import numpy as np
import gc
import cv2

# Local imports
from constants import BLUR_FACTOR, TARGET_RADIUS, Y_VAL, ALIGN, REFERENCE


def preprocess(packet):
    """ Run preprocessing on pair of images in place
    [in] packet : pipeline packet
    [post] images preprocessed in place
    [raise] ZeroDivisionError
    """
    # bit_scale(packet)
    dead_pixel_correction(packet)
    dark_current_correction(packet)
    packet.unload_dark()  # Dark no longer needed
    flat_fielding(packet)
    packet.unload_white()  # Flat no longer needed
    # registration(packet)


def dead_pixel_correction(packet):
    """ Correct for dark pixels by applying a median blur to the images
    [in] packet : pipeline packet
    [post] images corrected for dead pixels
    """
    subject = packet.get_subject()

    # If the Ws have not yet been generated, we need to correct the flats
    if packet.flat_field_ws == ():
        dark = packet.get_dark_img()
        white = packet.get_white_img()
        dark[0][...] = cv2.medianBlur(dark[0], BLUR_FACTOR)
        dark[1][...] = cv2.medianBlur(dark[1], BLUR_FACTOR)
        white[0][...] = cv2.medianBlur(white[0], BLUR_FACTOR)
        white[1][...] = cv2.medianBlur(white[1], BLUR_FACTOR)

    subject[0][...] = cv2.medianBlur(subject[0], BLUR_FACTOR)
    subject[1][...] = cv2.medianBlur(subject[1], BLUR_FACTOR)


def bit_scale(packet):
    """ Bit scale pair of images in place
    [in] packet : pipeline packet
    [post] images bit scaled in place
    """
    subject = packet.get_subject()

    s = ((2 ** 16 - 1) / (2 ** 14 - 1))  # TODO determine actual scale for each image

    # If the Ws have not yet been generated, we need to scale the flats
    if packet.flat_field_ws == ():
        dark = packet.get_dark_img()
        white = packet.get_white_img()
        dark[0][...] *= s
        dark[1][...] *= s
        white[0][...] *= s
        white[1][...] *= s

    subject[0][...] *= s
    subject[1][...] *= s


def dark_current_correction(packet):
    """ Dark current correct pair of images in place
    [in] packet : pipeline packet
    [post] images dark current corrected in place
    """
    subject = packet.get_subject()
    dark = packet.get_dark_img()

    # If the Ws have not yet been generated, we need to correct the flat fields
    if packet.flat_field_ws == ():
        white = packet.get_white_img()
        white[0][...] -= dark[0]
        white[1][...] -= dark[1]

    subject[0][...] -= dark[0]
    subject[1][...] -= dark[1]


def flat_field_w_gen(packet):
    """ Generate flat fielding w value
    [in] packet : pipeline packet
    """
    target = packet.target
    white = packet.get_white_img()
    t_img = packet.get_target_img()

    # Get mean values
    tr = TARGET_RADIUS
    row, col = target.get_white()
    xpos, ypos = target.get_center_coord(row, col)
    t1mean = np.mean(t_img[0][ypos - tr:ypos + tr, xpos - tr:xpos + tr], axis=(0, 1))
    t2mean = np.mean(t_img[1][ypos - tr:ypos + tr, xpos - tr:xpos + tr], axis=(0, 1))
    w1mean = np.mean(white[0][ypos - tr:ypos + tr, xpos - tr:xpos + tr], axis=(0, 1))
    w2mean = np.mean(white[1][ypos - tr:ypos + tr, xpos - tr:xpos + tr], axis=(0, 1))

    # Generate Ws
    w1 = Y_VAL * (t1mean / w1mean)
    w2 = Y_VAL * (t2mean / w2mean)
    packet.flat_field_ws = (w1, w2)


def flat_fielding(packet):
    """ Flat field image pair in place
    [in] packet : pipeline packet
    [raise] ZeroDivisionError
    """
    subject = packet.get_subject()
    white = packet.get_white_img()

    # Generate Ws if we haven't already
    if packet.flat_field_ws == ():
        flat_field_w_gen(packet)

    # Flat fielding
    subject[0][...] /= white[0]
    subject[0][...] *= packet.flat_field_ws[0]
    subject[1][...] /= white[1]
    subject[1][...] *= packet.flat_field_ws[1]


def registration(packet):
    """ Register image pair in place
    [in] packet : pipeline packet
    [post] images registered in place
    """
    import time
    t = time.perf_counter()
    subject = packet.get_subject()

    # grayscale
    img1_gray = cv2.cvtColor(subject[ALIGN], cv2.COLOR_RGB2GRAY)
    img2_gray = cv2.cvtColor(subject[REFERENCE], cv2.COLOR_RGB2GRAY)
    height, width = img2_gray.shape

    # convert to uint8 for detection
    img1_gray_norm = cv2.normalize(img1_gray, None, 0, 255, cv2.NORM_MINMAX).astype('uint8')
    img2_gray_norm = cv2.normalize(img2_gray, None, 0, 255, cv2.NORM_MINMAX).astype('uint8')
    del img1_gray, img2_gray
    gc.collect()

    # create SIFT detector with 2000 features
    detector = cv2.SIFT_create(2000)
    descriptor = cv2.SIFT_create(2000)

    # generate keypoints
    key_points1 = detector.detect(img1_gray_norm, None)
    key_points2 = detector.detect(img2_gray_norm, None)

    # TODO real errors
    if key_points1 is None or key_points2 is None:
        print("No key points created")
        exit()

    # generate descriptors
    key_points1, descriptors1 = descriptor.compute(img1_gray_norm, key_points1)
    key_points2, descriptors2 = descriptor.compute(img2_gray_norm, key_points2)
    del img1_gray_norm, img2_gray_norm
    gc.collect()

    # create matcher and match images, dropping bottom 10% of matches
    matcher = cv2.BFMatcher(cv2.NORM_L1, crossCheck=False)
    matches = matcher.match(descriptors1, descriptors2)
    del descriptors1, descriptors2
    gc.collect()
    matches = tuple(sorted(matches, key=lambda x: x.distance))
    matches = matches[:int(len(matches) * 0.9)]
    num_matches = len(matches)

    # set up arrays for calculating homography
    p1 = np.zeros((num_matches, 2))
    p2 = np.zeros((num_matches, 2))
    for i in range(len(matches)):
        p1[i, :] = key_points1[matches[i].queryIdx].pt
        p2[i, :] = key_points2[matches[i].trainIdx].pt
    del key_points1, key_points2, matches
    gc.collect()

    homography, mask = cv2.findHomography(p1, p2, cv2.RANSAC)

    # warp the subject images based on the calculated homography
    subject[1][...] = cv2.warpPerspective(subject[REFERENCE], homography, (width, height))

    print("register time: " + str(time.perf_counter() - t))
    del homography
    gc.collect()
    # sleep(10000000)
