""" target.py
Packet class definition

Classes:
    Packet

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import gc

# Local imports
from rgbio import load_image, load_array, save_array, create_temp_file
from constants import TARGET_A_IDX, TARGET_B_IDX, WHITE_A_IDX, WHITE_B_IDX, \
    DARK_A_IDX, DARK_B_IDX, RENDERABLES_START, WHITE_SWAP_IDX, DARK_SWAP_IDX, \
    TARGET_SWAP_IDX, NUM_SWAP_FILES


class Packet:
    """ Packet
    Class representing a single packet going through a pipeline

    Members:
        files         : List of files to be read in
        imgs          : Array of all images
        flat_field_ws : Tuple of w values from flat fielding
        target        : The calibration target
        swap          : List of files for loading and unloading arrays
        subj_idx      : Indices of the currently referenced image
        rendered_subj  : Rendered version of the subject
        x             : Calibration solution

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
    subj_idx = (TARGET_A_IDX, TARGET_B_IDX)
    rendered_subj = None
    x = []

    def load_calibration_imgs(self):
        """ Load images needed for calibration """
        for i in range(0, RENDERABLES_START):
            self.imgs.append(load_image(self.files[i]))

    def get_white_img(self):
        """ Get tuple of flat field images """
        return self.imgs[WHITE_A_IDX], self.imgs[WHITE_B_IDX]

    def get_dark_img(self):
        """ Get tuple of dark field images """
        return self.imgs[DARK_A_IDX], self.imgs[DARK_B_IDX]

    def get_target_img(self):
        """ Get tuple of target images """
        return self.imgs[TARGET_A_IDX], self.imgs[TARGET_B_IDX]

    def get_subject(self):
        """ Get the currently referenced image """
        return self.imgs[self.subj_idx[0]], self.imgs[self.subj_idx[1]]

    def generate_swap(self):
        """ Generate swap space
        [post] self.swap contains swap space file paths
        """
        for i in range(0, NUM_SWAP_FILES):
            self.swap.append(create_temp_file())

    def unload_white(self):
        """ Unload flat field images """
        self.__unload_imgs(WHITE_A_IDX, WHITE_B_IDX, WHITE_SWAP_IDX)

    def unload_dark(self):
        """ Unload flat field images """
        self.__unload_imgs(DARK_A_IDX, DARK_B_IDX, DARK_SWAP_IDX)

    def unload_target(self):
        """ Unload flat field images """
        self.__unload_imgs(TARGET_A_IDX, TARGET_B_IDX, TARGET_SWAP_IDX)

    def unload_subject(self):
        """ Unload currently referenced image """
        self.__unload_imgs(self.subj_idx[0], self.subj_idx[1])

    def load_white(self):
        """ Unload flat field images """
        self.__load_imgs(WHITE_A_IDX, WHITE_B_IDX, WHITE_SWAP_IDX)

    def load_dark(self):
        """ Unload flat field images """
        self.__load_imgs(DARK_A_IDX, DARK_B_IDX, DARK_SWAP_IDX)

    def load_target(self):
        """ Unload flat field images """
        self.__load_imgs(TARGET_A_IDX, TARGET_B_IDX, TARGET_SWAP_IDX)

    def load_subject(self):
        idx0, idx1 = self.subj_idx[0], self.subj_idx[1]
        if self.subj_idx[0] == TARGET_A_IDX:
            self.__load_imgs(idx0, idx1, TARGET_SWAP_IDX)
        else:
            self.__load_imgs(idx0, idx1)

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
            self.imgs[a] = load_image(a)
            self.imgs[b] = load_image(b)

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
