""" packet.py
Dataclasses and functions related to the packet

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
# Python Imports
import gc
import numpy as np
from dataclasses import dataclass

# Local Imports
from rgbio import load_image, load_array, save_array, create_temp_file
from constants import IMGTYPE_TARGET, IMGTYPE_WHITE,\
        IMGTYPE_DARK, IMGTYPE_SUBJECT, TARGET_RADIUS,\
        TARGETTYPE_NGT, TARGETTYPE_APT, TARGETTYPE_CCSG, TARGETTYPE_CC


# File/array index constants
__TARGET_A_IDX = 0
__TARGET_B_IDX = 1
__WHITE_A_IDX = 2
__WHITE_B_IDX = 3
__DARK_A_IDX = 4
__DARK_B_IDX = 5
RENDERABLES_START = 6

# Target shapes
__NGT_SHAPE = (10, 13)
__APT_SHAPE = (0, 6)
__CCSG_SHAPE = (0, 0)
__CC_SHAPE = (0, 0)

# TARGETTYPE2files
__ngtfiles = ('data/NGT_spectral_reflectance.csv', 'data/NGT_lab.csv')
__aptfiles = ('data/APT_spectral_reflectance.csv', 'data/APT_lab.csv')
__ccsgfiles = ('data/CCSG_spectral_reflectance.csv', 'data/CCSG_lab.csv')
__ccfiles = ('data/CC_spectral_reflectance.csv', 'data/CC_lab.csv')
__ttype2files = {TARGETTYPE_NGT: __ngtfiles,
                 TARGETTYPE_APT: __aptfiles,
                 TARGETTYPE_CCSG: __ccsgfiles,
                 TARGETTYPE_CC: __ccfiles}


@dataclass
class Packet:
    """ Packet
    Struct to hold pipeline data

    Members:
        files   : list of image files
        swap    : list of temp files for storing image arrays
        outpath : path to output files to
        subjptr : tuple containing indices of current subject for batch
        target  : dataclass for the target
        wscale  : white patch scale value
        mcalib  : calibrated matrix
        camsigs : TODO delete
    """
    files: list
    swap: list
    outpath: str
    subjptr: tuple
    target: np.ndarray
    wscale: tuple
    mcalib: np.ndarray
    camsigs: np.ndarray


@dataclass
class Target:
    """ Target
    Struct representing the target grid

    Members:
        tlcorner   : top left corner of the grid in image space (x,y)
        blcorner   : bottom right corner of the grid in image space (x,y)
        whitepatch : location of white patch in target space (row, col)
        shape      : dimension of target (row, col)
        r_ref      : reflectance reference
        lab_ref    : LAB reference
    """
    tlcorner: tuple
    blcorner: tuple
    whitepatch: tuple
    shape: tuple
    r_ref: np.ndarray
    lab_ref: np.ndarray


def genpacket(files: list, target: Target, outpath: str) -> Packet:
    """ Initialize packet with default values and loaded images
    images will be in swap after this
    [in] files  : list of files we are working with
    [in] target : target grid
    [out] packet
    """
    swap = __genswap(len(files) // 2)
    subjptr = (__TARGET_A_IDX, __TARGET_B_IDX)
    pkt = Packet(files, swap, outpath, subjptr,
                 target, (None, None), None, None)
    __loadswap(pkt)
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

    swapidx = __getswapidx(a)
    return __loadimg(packet, a, b, swapidx)


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


def gentarget(coords: tuple, wpatch: tuple, targettype: int) -> Target:
    """ Initialize target
    [in] coords     : target coordinates(topleft(x,y), bottomright(x,y))
    [in] wpatch     : white patch location (row, col)
    [in] targettype : target type
    """
    if targettype == TARGETTYPE_NGT:
        shape = __NGT_SHAPE
    else:
        # TODO error handling
        print("Invalid target type")
        exit(-1)

    target = Target(coords[0], coords[1], wpatch, shape, None, None)
    __loadrefs(target, targettype)
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
            siglist.append(__getpatchloc(target, r, c))
    return siglist


def genwhitepatchxy(target: Target) -> tuple:
    """ Generate a coordinate for the white patch
    [in] target : the target we are operating on
    [out] white patch center coordinate (x,y)
    """
    return __getpatchloc(target, target.whitepatch[0], target.whitepatch[1])


def extract_camsigs(packet):
    """ Generate camsigs array
    [in] packet : pipeline packet
    [out] camsigs array
    """
    t_img = getimg(packet, IMGTYPE_TARGET)
    siglist = genpatchlist(packet.target)
    tr = TARGET_RADIUS
    camsigs = np.ndarray((6, 130))
    for i, sig in enumerate(siglist):
        cell = t_img[0][sig[1]-tr:sig[1]+tr, sig[0]-tr:sig[0]+tr]
        avg = np.average(cell, axis=(0, 1))
        camsigs[0, i] = avg[0]
        camsigs[1, i] = avg[1]
        camsigs[2, i] = avg[2]
        cell = t_img[1][sig[1]-tr:sig[1]+tr, sig[0]-tr:sig[0]+tr]
        avg = np.average(cell, axis=(0, 1))
        camsigs[3, i] = avg[0]
        camsigs[4, i] = avg[1]
        camsigs[5, i] = avg[2]

    return camsigs


def __loadswap(packet: Packet):
    """ Populate swap files with their corresponding images
    [in] packet : packet we are operating on
    [post] temp files have been loaded with the images
    """
    for i, s in enumerate(packet.swap):
        # Gen a,b indices into file list
        a = i * 2
        b = a + 1
        # Load files
        aimg, bimg = __loadimg(packet, a, b)
        # Save in swap
        save_array((aimg, bimg), s)


def __getswapidx(idx: int) -> int:
    """ Get index of corresponding swap file
    [in] idx : index of image in file list
    [out] index of image is swap list
    """
    return idx // 2


def __genswap(n: int) -> list:
    """ Generate swapfiles
    [in] n : number of files to generate
    [out] swap list
    """
    swap = []
    for i in range(0, n):
        swap.append(create_temp_file())
    return swap


def __loadimg(packet: Packet, a: int, b: int, s: int = None) -> tuple:
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


def __getpatchloc(target: Target, row: int, col: int) -> tuple:
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


def __loadrefs(target: Target, targettype: int):
    files = __ttype2files[targettype]
    # Reflectance
    ref = np.genfromtxt(files[0], delimiter=',')
    target.r_ref = np.reshape(ref, (36, ref.shape[0]//36))
    # LAB
    ref = np.genfromtxt(files[1], delimiter=',')
    target.lab_ref = np.reshape(ref, (3, ref.shape[0]//3))
    print(target.lab_ref)
    import lab_refs
    print(lab_refs.LAB_REF)
