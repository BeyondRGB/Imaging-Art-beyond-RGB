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

from packet import Packet, getimg, putimg, genwhitepatchxy
from constants import TARGET_RADIUS, IMGTYPE_WHITE,\
        IMGTYPE_DARK, IMGTYPE_SUBJECT


__BLUR_FACTOR = 3  # Dead pixel correction
__YVAL = 0.86122  # Flat fielding


def preprocess(packet: Packet):
    """ Run preprocessing on pair of images in place
    [in] packet : pipeline packet
    [post] images preprocessed in place
    """
    subj = getimg(packet, IMGTYPE_SUBJECT)
    white = getimg(packet, IMGTYPE_WHITE)
    dark = getimg(packet, IMGTYPE_DARK)

    if packet.wscale[0] is None:
        # This is out first time through
        __deadpixels(subj, dark, white)
        __darkcurrent(subj, dark, white)
        __wscalegen(packet, subj, white)
        __flatfield(packet, subj, white)
        putimg(packet, IMGTYPE_DARK, dark)  # save once
        putimg(packet, IMGTYPE_WHITE, white)  # save once
    else:
        # Preprocessing on non target images
        __deadpixels(subj)
        __darkcurrent(subj, dark)
        putimg(packet, IMGTYPE_DARK, dark)
        __flatfield(packet, subj, white)
        del dark, white
        gc.collect()

    putimg(packet, IMGTYPE_SUBJECT, subj)  # Always save subject


def __deadpixels(subj: tuple, dark: tuple = None, white: tuple = None):
    """ Correct for dark pixels by applying a median blur to the images
    [in] subj      : subject images
    [in,opt] white : white images (only used for the first pass)
    [in,opt] dark  : dark images (only used for the first pass)
    [post] images corrected for dead pixels
    """
    if white:
        white[0][...] = medianBlur(white[0], __BLUR_FACTOR)
        white[1][...] = medianBlur(white[1], __BLUR_FACTOR)
    if dark:
        dark[0][...] = medianBlur(dark[0], __BLUR_FACTOR)
        dark[1][...] = medianBlur(dark[1], __BLUR_FACTOR)
    subj[0][...] = medianBlur(subj[0], __BLUR_FACTOR)
    subj[1][...] = medianBlur(subj[1], __BLUR_FACTOR)


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

    t0mean = np.mean(target[0][(y - tr):(y + tr), (x - tr):(x + tr)], (0, 1))
    t1mean = np.mean(target[1][(y - tr):(y + tr), (x - tr):(x + tr)], (0, 1))
    w0mean = np.mean(white[0][(y - tr):(y + tr), (x - tr):(x + tr)], (0, 1))
    w1mean = np.mean(white[1][(y - tr):(y + tr), (x - tr):(x + tr)], (0, 1))

    # TODO dynamic YVAL generation
    w0 = __YVAL * (t0mean / w0mean)
    w1 = __YVAL * (t1mean / w1mean)
    packet.wscale = (w0, w1)


def __flatfield(packet: Packet, subj: tuple, white: tuple):
    """ Flat field image pair
    [in] packet  : pipeline packet
    [in] subj    : subject images
    [in] white   : white images
    [post] subject flat fielded
    """
    subj[0][...] /= white[0]
    subj[0][...] *= packet.wscale[0]
    subj[1][...] /= white[1]
    subj[1][...] *= packet.wscale[1]


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
