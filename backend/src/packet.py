""" target.py
Packet struct and related functions

Classes:
    Packet

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import gc

from rgbio import load_image, load_array, save_array, create_temp_file

""" Constants """
# File/array index constants
__TARGET_A_IDX = 0
__TARGET_B_IDX = 1
__WHITE_A_IDX = 2
__WHITE_B_IDX = 3
__DARK_A_IDX = 4
__DARK_B_IDX = 5
__RENDERABLES_START = 6
# Swap space index constants
__WHITE_SWAP_IDX = 0
__DARK_SWAP_IDX = 1
__TARGET_SWAP_IDX = 2
__NUM_SWAP_PAIRS = 3
__NUM_SWAP_FILES = __NUM_SWAP_PAIRS * 2


    """ Packet
    Class representing a single packet going through a pipeline

    Members:
        files         : List of files to be read in
        imgs          : Array of all images
        flat_field_ws : Tuple of w values from flat fielding
        target        : The calibration target
        swap          : List of files for loading and unloading arrays
        subj_idx      : Indices of the currently referenced image
        x             : Calibration solution
        dims          : Dimentions the final image should have
        camsigs       : Camera signals list of image being processed
        render        : Render of current subject

    Methods:
        load_calibration_imgs : Load images needed for calibration
        get_white_img         : Get tuple of flat field images
        get_dark_img          : Get tuple of dark field images
        get_target_img        : Get tuple of target images
        generate_swap         : Generate swap space
        unload_white          : Unload flat field images
        unload_dark           : Unload dark field images
        unload_target         : Unload target images
    """
    files = []
    imgs = []
    flat_field_ws = ()
    target = None
    swap = []
    subject_idx = ()
    x = []
    dims = ()
    camsigs = []
    render = []

    def __init__(self):
        self.subject_idx = (__TARGET_A_IDX, __TARGET_B_IDX)

    def delcamsigs(self):
        """ Delete camsigs; values not needed """
        del self.camsigs
        gc.collect()

    def delrendervars(self):
        """ Delete variables used in rendering for next render """
        del self.render
        del self.dims
        gc.collect()

    def load_calibration_imgs(self):
        """ Load images needed for calibration """
        for i in range(0, __RENDERABLES_START):
            self.imgs.append(load_image(self.files[i]))

    def get_white_img(self):
        """ Get tuple of flat field images """
        return self.imgs[__WHITE_A_IDX], self.imgs[__WHITE_B_IDX]

    def get_dark_img(self):
        """ Get tuple of dark field images """
        return self.imgs[__DARK_A_IDX], self.imgs[__DARK_B_IDX]

    def get_target_img(self):
        """ Get tuple of target images """
        return self.imgs[__TARGET_A_IDX], self.imgs[__TARGET_B_IDX]

    def get_subject(self):
        """ Get the currently referenced image """
        return self.imgs[self.subject_idx[0]], self.imgs[self.subject_idx[1]]

    def generate_swap(self):
        """ Generate swap space
        [post] self.swap contains swap space file paths
        """
        for i in range(0, __NUM_SWAP_FILES):
            self.swap.append(create_temp_file())

    def unload_white(self):
        """ Unload flat field images """
        self.__unload_imgs(__WHITE_A_IDX, __WHITE_B_IDX, __WHITE_SWAP_IDX)

    def unload_dark(self):
        """ Unload flat field images """
        self.__unload_imgs(__DARK_A_IDX, __DARK_B_IDX, __DARK_SWAP_IDX)

    def unload_target(self):
        """ Unload flat field images """
        self.__unload_imgs(__TARGET_A_IDX, __TARGET_B_IDX, __TARGET_SWAP_IDX)

    def unload_subject(self):
        """ Unload currently referenced image """
        self.__unload_imgs(self.subject_idx[0], self.subject_idx[1])

    def load_white(self):
        """ Unload flat field images """
        self.__load_imgs(__WHITE_A_IDX, __WHITE_B_IDX, __WHITE_SWAP_IDX)

    def load_dark(self):
        """ Unload flat field images """
        self.__load_imgs(__DARK_A_IDX, __DARK_B_IDX, __DARK_SWAP_IDX)

    def load_target(self):
        """ Unload flat field images """
        self.__load_imgs(__TARGET_A_IDX, __TARGET_B_IDX, __TARGET_SWAP_IDX)

    def load_subject(self):
        self.__load_imgs(self.subject_idx[0], self.subject_idx[1])

    def __load_imgs(self, a, b, s=None):
        """ Load image pair
        [in] a      : A file index
        [in] b      : B file index
        [in, opt] s : swap file location index
        [post] image pair loaded into indices
        """
        if s:
            self.imgs[a], self.imgs[b] = load_array(self.swap[s])
        else:
            self.imgs[a] = load_image(self.files[self.subject_idx[0]])
            self.imgs[b] = load_image(self.files[self.subject_idx[1]])

    def __unload_imgs(self, a, b, s=None):
        """ Unload image pair
        [in] a      : A file index
        [in] b      : B file index
        [in, opt] s : swap file idx
        [post] image pair deleted and replaced with None
        """
        # Save images
        if s:
            save_array((self.imgs[a], self.imgs[b]), self.swap[s])

        # Delete from memory
        del self.imgs[b], self.imgs[a]
        gc.collect()

        # Replace with None
        self.imgs.insert(a, None)
        self.imgs.insert(b, None)
