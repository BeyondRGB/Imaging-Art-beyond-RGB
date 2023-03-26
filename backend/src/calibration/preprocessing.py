""" preprocessing.py
Collection of functions used for preprocessing

Functions:
    preprocess(packet)
    dark_current_correction(packet, dimgs)
    flat_field_w_gen(packet)
    flat_fielding(packet)

Authors:
    Brendan Grau <https://github.com/Victoriam7>
    Elijah Cooper

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import gc
import numpy as np
from cv2 import medianBlur

from calibration.packet import Packet, getimg, putimg, genwhitepatchxy
from calibration.constants import TARGET_RADIUS, IMGTYPE_WHITE,\
                                  IMGTYPE_DARK, IMGTYPE_SUBJECT


__BLUR_FACTOR = 3  # Dead pixel correction


def preprocess(packet: Packet):
    """ Run preprocessing on pair of images in place
    [in] packet : pipeline packet
    [post] images preprocessed in place
    """
    subj = getimg(packet, IMGTYPE_SUBJECT)
    white = getimg(packet, IMGTYPE_WHITE)
    dark = getimg(packet, IMGTYPE_DARK)

    if packet.wscale == 0.0:
        # This is out first time through
        __bitscale(subj, dark, white)
        __deadpixels(packet, subj, dark, white)
        __darkcurrent(subj, dark, white)
        __wscalegen(packet, subj, white)
        __flatfield(packet, subj, white)
        putimg(packet, IMGTYPE_DARK, dark)  # save once
        putimg(packet, IMGTYPE_WHITE, white)  # save once
    else:
        # Preprocessing on non target images
        __bitscale(subj)
        __deadpixels(packet, subj)
        __darkcurrent(subj, dark)
        __flatfield(packet, subj, white)
        del dark, white
        gc.collect()

    putimg(packet, IMGTYPE_SUBJECT, subj)  # Always save subject


def __bitscale(subj: tuple, dark: tuple = None, white: tuple = None):
    """ Scale pixels up to 16 bit
    [in] subj      : subject images
    [in,opt] white : white images (only used for the first pass)
    [in,opt] dark  : dark images (only used for the first pass)
    [post] images scaled to 16 bit
    """
    s = ((2.0**16.0)-1.0)/((2.0**14.0)-1.0)
    if white:
        white[0][...] *= s
        white[1][...] *= s
    if dark:
        dark[0][...] *= s
        dark[1][...] *= s
    subj[0][...] *= s
    subj[1][...] *= s


def __deadpixels(packet: Packet, subj: tuple, white: tuple = None, dark: tuple = None):
    """ Correct for dark pixels by applying a median blur to the images
    [in] packet   : pipeline packet
    [in] subj     : subject images
    [in,opt] white : white images (only used for the first pass)
    [in,opt] dark  : dark images (only used for the first pass)
    [post] images corrected for dead pixels
    """
    # Get/generate indicies of dead pixels
    if packet.deadpixels:
        idxs0, idxs1 = packet.deadpixels
    else:
        idxs0 = np.where(white[0] - dark[0] == 0)
        idxs1 = np.where(white[1] - dark[1] == 0)

    # Blur image and then only save values for dead pixels
    if white:
        blur = medianBlur(white[0], __BLUR_FACTOR)
        white[0][idxs0] = blur[idxs0]
        blur = medianBlur(white[1], __BLUR_FACTOR)
        white[1][idxs1] = blur[idxs1]
    if dark:
        blur = medianBlur(dark[0], __BLUR_FACTOR)
        dark[0][idxs0] = blur[idxs0]
        blur = medianBlur(dark[1], __BLUR_FACTOR)
        dark[1][idxs1] = blur[idxs1]
    blur = medianBlur(subj[0], __BLUR_FACTOR)
    subj[0][idxs0] = blur[idxs0]
    blur = medianBlur(subj[1], __BLUR_FACTOR)
    subj[1][idxs1] = blur[idxs1]

    del blur
    gc.collect()

    # Store in packet for later images
    packet.deadpixels = (idxs0, idxs1)


def __darkcurrent(subj: tuple, dark: tuple, white: tuple = None):
    """ Dark current correct pair of images
    [in] subj      : subject images
    [in] dark      : dark images
    [in,opt] white : white images (only used for the first pass)
    [post] images corrected for camera dark current
    """
    if white:
        white[0][...] -= dark[0]
        white[1][...] -= dark[1]
    subj[0][...] -= dark[0]
    subj[1][...] -= dark[1]


def __wscalegen(packet: Packet, target: tuple, white: tuple):
    """ Generate flat fielding w value
    [in] packet : pipeline packet
    [in] target : target images
    [in] white  : white images
    [post] packet.wscale populated
    """
    tr = TARGET_RADIUS
    x, y = genwhitepatchxy(packet.target)

    # Gather average of white patch area defined by +- TARGET_RADIUS
    # We only need the green channel
    tmean = np.average(target[0][(y - tr):(y + tr), (x - tr):(x + tr)], (0, 1))[1]
    wmean = np.average(white[0][(y - tr):(y + tr), (x - tr):(x + tr)], (0, 1))[1]

    # Get Y value
    target = packet.target
    row, col = target.whitepatch
    numrows, numcols = target.shape
    idx = col * numrows + row  # convert 2d -> 1d in column major
    yval = target.xyz_ref[1][idx] / 100

    w = yval * (wmean / tmean)
    packet.wscale = w


def __flatfield(packet: Packet, subj: tuple, white: tuple):
    """ Flat field image pair
    [in] packet  : pipeline packet
    [in] subj    : subject images
    [in] white   : white images
    [post] subject flat fielded
    """
    subj[0][...] /= white[0]
    subj[0][...] *= packet.wscale
    subj[1][...] /= white[1]
    subj[1][...] *= packet.wscale


"""
def registration(packet):
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
"""
