""" target.py
dataclasses and functions related to the packet

Structs:
    Packet
    Target

Functions:
    genpacket       : Initialize packet with default values
    getimg          : Get specified image
    putimg          : Put image back in swap
    gentarget       : Initialize target
    genpatchlist    : Generate the list of target patches
    genwhitepatchxy : Generate a coordinate for the white patch

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import gc
import numpy as np
from dataclasses import dataclass

from rgbio import load_image, load_array, save_array, create_temp_file
from constants import IMGTYPE_TARGET, IMGTYPE_WHITE,\
        IMGTYPE_DARK, IMGTYPE_SUBJECT


# File/array index constants
__TARGET_A_IDX = 0
__TARGET_B_IDX = 1
__WHITE_A_IDX = 2
__WHITE_B_IDX = 3
__DARK_A_IDX = 4
__DARK_B_IDX = 5
__RENDERABLES_START = 6


@dataclass
class Packet:
    """ Packet
    Struct to hold pipeline data

    Members:
        files   : list of image files
        swap    : list of temp files for storing image arrays
        subjptr : tuple containing indices of current subject for batch
        target  : dataclass for the target
        wscale  : white patch scale value
        mcalib  : calibrated matrix
        dims    : TODO delete
        camsigs : TODO delete
        render  : TODO delete
    """
    files: list
    swap: list
    subjptr: tuple
    target: np.ndarray
    wscale: tuple
    mcalib: np.ndarray
    camsigs: np.ndarray
    render: np.ndarray


@dataclass
class Target:
    """ Target
    Struct representing the target grid

    Members:
        tlcorner   : top left corner of the grid in image space (x,y)
        blcorner   : bottom right corner of the grid in image space (x,y)
        whitepatch : location of white patch in target space (row, col)
        shape      : dimension of target (row, col)
    """
    tlcorner: tuple
    blcorner: tuple
    whitepatch: tuple
    shape: tuple


def genpacket(files: list, target: Target) -> Packet:
    """ Initialize packet with default values and loaded images
    images will be in swap after this
    [in] files  : list of files we are working with
    [in] target : target grid
    [out] packet
    """
    swap = __swapgen(len(files) // 2)
    subjptr = (__TARGET_A_IDX, __TARGET_B_IDX)
    pkt = Packet(files, swap, subjptr, target, (None, None), None, None, None)
    __swapload(pkt)
    return pkt


def getimg(packet: Packet, imgtype: int) -> tuple:
    """ Get specified image
    Loads image from swap space and returns it
    [in] packet  : packet we are operating on
    [in] imgtype : image type as specified in constants
    [out] tuple containing specified (A, B) image pair
    """
    if imgtype == IMGTYPE_TARGET:
        a, b = __TARGET_A_IDX, __TARGET_B_IDX
    elif imgtype == IMGTYPE_WHITE:
        a, b = __WHITE_A_IDX, __WHITE_B_IDX
    elif imgtype == IMGTYPE_DARK:
        a, b = __DARK_A_IDX, __DARK_B_IDX
    elif imgtype == IMGTYPE_SUBJECT:
        a, b = packet.subjptr[0], packet.subjptr[1]
    else:
        return None, None

    swapidx = __swapidxget(a)
    return __imgload(packet, a, b, swapidx)


def putimg(packet: Packet, imgtype: int, imgpair: tuple):
    """ Put image back in swap
    only call if image contents need to be saved otherwise just delete
    [in] packet  : packet we are operating on
    [in] imgtype : image type as specified in constants
    [in] imgpair : image pair to be saved
    """
    if imgtype == IMGTYPE_TARGET:
        s = __TARGET_A_IDX // 2
    elif imgtype == IMGTYPE_WHITE:
        s = __WHITE_A_IDX // 2
    elif imgtype == IMGTYPE_DARK:
        s = __DARK_A_IDX // 2
    elif imgtype == IMGTYPE_SUBJECT:
        s = packet.subjptr[0] // 2
    else:
        return

    save_array(imgpair, packet.swap[s])

    del imgpair
    gc.collect()


def gentarget(tlcorner: tuple, brcorner: tuple, whitepatch: tuple) -> Target:
    """ Initialize target
    [in] tlcorner   : the top left coordinate of the target (x, y)
    [in] brcorner   : the bottom right coordinate of the target (x, y)
    [in] whitepatch : white patch location (row, col)
    """
    target = Target(tlcorner, brcorner, whitepatch, (10, 13))
    return target


def genpatchlist(target: Target) -> list:
    """ Generate the list of target patches
    [in] target : the target we are operating on
    [out] list of patch center points
    """
    siglist = []
    rows, cols = target.shape
    for c in range(0, cols):
        for r in range(0, rows):
            siglist.append(__patchlocgen(target, r, c))
    return siglist


def genwhitepatchxy(target: Target) -> tuple:
    """ Generate a coordinate for the white patch
    [in] target : the target we are operating on
    [out] white patch center coordinate (x,y)
    """
    return __patchlocgen(target, target.whitepatch[0], target.whitepatch[1])


def __swapload(packet: Packet):
    """ Populate swap files with their corresponding images
    [in] packet : packet we are operating on
    [post] temp files have been loaded with the images
    """
    for i, s in enumerate(packet.swap):
        # Gen a,b indices into file list
        a = i * 2
        b = a + 1
        # Load files
        aimg, bimg = __imgload(packet, a, b)
        # Save in swap
        save_array((aimg, bimg), s)


def __swapidxget(idx: int) -> int:
    """ Get index of corresponding swap file
    [in] idx : index of image in file list
    [out] index of image is swap list
    """
    return idx // 2


def __swapgen(n: int) -> list:
    """ Generate swapfiles
    [in] n : number of files to generate
    [out] swap list
    """
    swap = []
    for i in range(0, n):
        swap.append(create_temp_file())
    return swap


def __imgload(packet: Packet, a: int, b: int, s: int = None) -> tuple:
    """ Load image pair
    [in] packet : packet we are operating on
    [in] a      : A file index
    [in] b      : B file index
    [in, opt] s : swap file location index
    [out] image pair in (A, B) tuple
    """
    if s is not None:
        return load_array(packet.swap[s])
    else:
        aimg = load_image(packet.files[a])
        bimg = load_image(packet.files[b])
        return aimg, bimg


def __patchlocgen(target: Target, row: int, col: int) -> tuple:
    """ Get the center of a target square
    [in] row : target square row
    [in] col : target square column
    [out] center coordinate of target square
    """
    tl = target.tlcorner
    br = target.blcorner
    rows, cols = target.shape
    # Calculate offset
    x = (col*2+1)*((br[0]-tl[0])/(cols*2))
    y = (row*2+1)*((br[1]-tl[1])/(rows*2))

    # Calculate true center
    x += tl[0]
    y += tl[1]

    return int(x), int(y)
