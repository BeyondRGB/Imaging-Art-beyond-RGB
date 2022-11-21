""" pipelines.py
Collection of all pipelines

Functions:
    processing_pipeline(files)

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import gc

# Local imports
from backend.src.preprocessing import registration
from rgbio import load_image, load_array, save_array, create_temp_file
from exceptions import MissingFilesException
from constants import IMAGE_A_IDX, IMAGE_B_IDX, WHITE_A_IDX, WHITE_B_IDX, \
    DARK_A_IDX, DARK_B_IDX, RENDERABLES_START, WHITE_SWAP_IDX, DARK_SWAP_IDX


def processing_pipeline(files):
    """ main color calibration pipeline
    [in] files : list of image file location touples ([(imgA, imgB)])
    [out] TODO
    [raise] IOError, FileNotFoundError, MissingFilesException
    """
    imgs = []
    swap = []
    num_files = len(files)

    # Validate request
    if num_files < 8:
        raise MissingFilesException(8, num_files)

    # Generate array swap space
    swap.append(create_temp_file())
    swap.append(create_temp_file())

    # Load files for calibration
    for i in range(0, RENDERABLES_START):
        imgs.append(load_image(files[i]))

    # Save flat and dark field arrays to disk
    save_array((imgs[WHITE_A_IDX], imgs[WHITE_B_IDX]), swap[WHITE_SWAP_IDX])
    save_array((imgs[DARK_A_IDX], imgs[DARK_B_IDX]), swap[DARK_SWAP_IDX])

    # TODO preprocess

    # Delete flat and dark field arrays from memory
    # Arrays must be deleted in the reverse order they are stored in the list
    del imgs[DARK_B_IDX], imgs[DARK_A_IDX]
    del imgs[WHITE_B_IDX], imgs[WHITE_A_IDX]
    gc.collect()

    # TODO registration
    imgs = ()
    registration(imgs)
    # TODO calibration

    # Delete targets from memory
    del imgs[IMAGE_B_IDX], imgs[IMAGE_A_IDX]
    gc.collect()

    # Reset imgs list
    imgs = [None] * 6

    # Load flat and dark field arrays into memory
    imgs[WHITE_A_IDX], imgs[WHITE_B_IDX] = load_array(swap[WHITE_SWAP_IDX])
    imgs[DARK_A_IDX], imgs[DARK_B_IDX] = load_array(swap[DARK_SWAP_IDX])

    # TODO rendering

    # TODO output
