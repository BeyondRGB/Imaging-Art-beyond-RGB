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
    Elija Cooper

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import numpy as np
from cv2 import medianBlur, cvtColor, COLOR_BGR2GRAY, BFMatcher,\
        findHomography, warpPerspective, RANSAC, ORB_create

# Local imports
from constants import BLUR_FACTOR, TARGET_RADIUS, Y_VAL


def preprocess(packet):
    """ Run preprocessing on pair of images in place
    [in] packet : pipeline packet
    [post] images preprocessed in place
    [raise] ZeroDivisionError
    """
#    bit_scale(packet)
    dead_pixel_correction(packet)
    dark_current_correction(packet)
    packet.unload_dark()  # Dark no longer needed
    flat_fielding(packet)
    packet.unload_white()  # Flat no longer needed
    registration(packet)


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
        dark[0][...] = medianBlur(dark[0], BLUR_FACTOR)
        dark[1][...] = medianBlur(dark[1], BLUR_FACTOR)
        white[0][...] = medianBlur(white[0], BLUR_FACTOR)
        white[1][...] = medianBlur(white[1], BLUR_FACTOR)

    subject[0][...] = medianBlur(subject[0], BLUR_FACTOR)
    subject[1][...] = medianBlur(subject[1], BLUR_FACTOR)


def bit_scale(packet):
    """ Bit scale pair of images in place
    [in] packet : pipeline packet
    [post] images bit scaled in place
    """
    subject = packet.get_subject()

    s = ((2**16 - 1)/(2**14 - 1))  # TODO determine actual scale for each image

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
    t1mean = np.mean(t_img[0][ypos-tr:ypos+tr, xpos-tr:xpos+tr], axis=(0, 1))
    t2mean = np.mean(t_img[1][ypos-tr:ypos+tr, xpos-tr:xpos+tr], axis=(0, 1))
    w1mean = np.mean(white[0][ypos-tr:ypos+tr, xpos-tr:xpos+tr], axis=(0, 1))
    w2mean = np.mean(white[1][ypos-tr:ypos+tr, xpos-tr:xpos+tr], axis=(0, 1))

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
    return  # TODO remove
    # TODO how are these gonna be and what to return
    reference_color, align_color = packet

    # grayscale
    img1 = cvtColor(reference_color, COLOR_BGR2GRAY)
    img2 = cvtColor(align_color, COLOR_BGR2GRAY)
    height, width = img2.shape

    # create ORB detector
    orb_detector = ORB_create(5000)
    key_points1, descriptors1 = orb_detector.detectAndCompute(img1, None)
    key_points2, descriptors2 = orb_detector.detectAndCompute(img2, None)

    # match images
    matches = BFMatcher.match(descriptors1, descriptors2)
    matches = tuple(sorted(matches, key=lambda x: x.distance))
    matches = matches[:int(len(matches)) * 0.9]
    num_matches = matches

    p1 = np.zeros((num_matches, 2))
    p2 = np.zeros((num_matches, 2))

    for i in range(len(matches)):
        p1[i, :] = key_points1[matches[i].queryIdx].pt
        p2[i, :] = key_points2[matches[i].trainIdx].pt

    homography, mask = findHomography(p1, p2, RANSAC)

    return warpPerspective(reference_color, homography, (width, height))
