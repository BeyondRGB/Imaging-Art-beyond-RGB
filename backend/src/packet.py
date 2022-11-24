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
        target        : Array of tuples (top right, bottom left) for targets
        swap          : List of files for loading and unloading arrays
        subj_idx      : indices of the currently referenced image

    Methods:
        load_calibration_imgs : Load images needed for calibration
        get_white             : Get tuple of flat field images
        get_dark              : Get tuple of dark field images
        get_target            : Get tuple of target images
        generate_swap         : Generate swap space
        unload_white          : Unload flat field images
        unload_dark           : Unload dark field images
        unload_target         : Unload target images
    """
    files = []
    imgs = []
    flat_field_ws = ()
    target = []
    swap = []
    subject_idx = (TARGET_A_IDX, TARGET_B_IDX)

    def load_calibration_imgs(self):
        """ Load images needed for calibration """
        for i in range(0, RENDERABLES_START):
            self.imgs.append(load_image(self.files[i]))

    def get_white(self):
        """ Get tuple of flat field images """
        return self.imgs[WHITE_A_IDX], self.imgs[WHITE_B_IDX]

    def get_dark(self):
        """ Get tuple of dark field images """
        return self.imgs[DARK_A_IDX], self.imgs[DARK_B_IDX]

    def get_target(self):
        """ Get tuple of target images """
        return self.imgs[TARGET_A_IDX], self.imgs[TARGET_B_IDX]

    def get_subject(self):
        """ Get the currently referenced image """
        return self.imgs[self.subject_idx[0]], self.imgs[self.subject_idx[1]]

    def generate_swap(self):
        """ Generate swap space
        [post] self.swap contains swap space file paths
        """
        for i in range(0, NUM_SWAP_FILES):
            self.swap.append(create_temp_file())

    def unload_white(self):
        """ Unload flat field images """
        self.__unload_imgs(WHITE_A_IDX, WHITE_B_IDX)

    def unload_dark(self):
        """ Unload flat field images """
        self.__unload_imgs(DARK_A_IDX, DARK_B_IDX)

    def unload_target(self):
        """ Unload flat field images """
        self.__unload_imgs(TARGET_A_IDX, TARGET_B_IDX)

    def unload_subject(self):
        """ Unload currently referenced image """
        # No saving, just delete
        del self.imgs[self.subj_idx[0]], self.imgs[self.subj_idx[1]]
        gc.collect()

        self.imgs.insert(self.subj_idx[0], None)
        self.imgs.insert(self.subj_idx[1], None)

    def load_white(self):
        """ Unload flat field images """
        self.__load_imgs(WHITE_A_IDX, WHITE_B_IDX, WHITE_SWAP_IDX)

    def load_dark(self):
        """ Unload flat field images """
        self.__load_imgs(DARK_A_IDX, DARK_B_IDX, DARK_SWAP_IDX)

    def load_target(self):
        """ Unload flat field images """
        self.__load_imgs(TARGET_A_IDX, TARGET_B_IDX, TARGET_SWAP_IDX)

    def __save_flat_dark(self):
        """ Save flat and dark field arrays to disk """
        save_array(self.get_white(), self.swap[WHITE_SWAP_IDX])
        save_array(self.get_dark(), self.swap[DARK_SWAP_IDX])

    def __load_imgs(self, a, b, s):
        """ Load image pair
        [in] a : A file index
        [in] b : B file index
        [in] s : swap file location index
        [post] image pair loaded into indices
        """
        self.imgs[a], self.imgs[b] = load_array(self.swap[s])

    def __unload_imgs(self, a, b):
        """ Unload image pair
        [in] a : A file index
        [in] b : B file index
        [post] image pair deleted and replaced with None
        """
        # Save images
        self.__save_flat_dark()

        # Delete from memory
        del self.imgs[DARK_B_IDX], self.imgs[DARK_A_IDX]
        gc.collect()

        # Replace with None
        self.imgs.insert(DARK_A_IDX, None)
        self.imgs.insert(DARK_B_IDX, None)
